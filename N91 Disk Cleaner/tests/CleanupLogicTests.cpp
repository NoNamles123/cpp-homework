#include <fstream>
#include <iostream>

#include "../DiskCleaner.cpp"

namespace {

int g_failures = 0;

void Expect(bool condition, const char* message) {
    if (condition) return;
    ++g_failures;
    std::cerr << "FAIL: " << message << '\n';
}

void WriteTestFile(const fs::path& path, size_t bytes) {
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    const std::string data(bytes, 'N');
    output.write(data.data(), static_cast<std::streamsize>(data.size()));
    output.close();
    if (!output) throw std::runtime_error("failed to create test file");
}

struct TestDirectory {
    fs::path path;

    ~TestDirectory() {
        std::error_code ec;
        fs::remove_all(path, ec);
    }
};

void TestIssueClassificationAndGrouping() {
    const auto systemError = [](DWORD code) {
        return std::error_code(static_cast<int>(code), std::system_category());
    };

    Expect(ClassifyFilesystemIssue(systemError(ERROR_FILE_NOT_FOUND), CleanupIssueKind::MetadataFailure) ==
               CleanupIssueKind::AlreadyGone,
           "missing files must be skipped");
    Expect(ClassifyFilesystemIssue(systemError(ERROR_SHARING_VIOLATION), CleanupIssueKind::MetadataFailure) ==
               CleanupIssueKind::BusyFile,
           "busy files must be skipped");
    Expect(ClassifyFilesystemIssue(systemError(ERROR_ACCESS_DENIED), CleanupIssueKind::MetadataFailure) ==
               CleanupIssueKind::AccessDenied,
           "access-denied files must be skipped");
    Expect(ClassifyFilesystemIssue(systemError(ERROR_CANT_ACCESS_FILE), CleanupIssueKind::MetadataFailure) ==
               CleanupIssueKind::UnavailableEntry,
           "Win32 error 1920 must be an unavailable skip");

    CleanupStats grouped;
    for (int index = 0; index < 25; ++index) {
        RecordCleanupIssue(grouped, CleanupIssueKind::MetadataFailure, true,
                           L"synthetic entry", ERROR_INVALID_DATA);
    }
    Expect(grouped.errors == 1, "one issue kind in one category must count as one real error");
    Expect(CleanupIssueCount(grouped, CleanupIssueKind::MetadataFailure) == 25,
           "all diagnostic occurrences must be retained in the bucket total");
    Expect(grouped.issues[CleanupIssueIndex(CleanupIssueKind::MetadataFailure)].samples.size() ==
               CLEANUP_LOG_LIMIT_PER_TYPE,
           "diagnostic samples must be capped at twenty per type");
}

void TestRuntimeLocalization() {
    const localization::Language original = localization::GetLanguage();

    localization::SetLanguage(localization::Language::English);
    Expect(loc::ui::Scan.str() == L"Scan", "English UI string must be selected");
    Expect(loc::Files(1) == L"1 file", "English singular form must be correct");
    Expect(loc::Files(2) == L"2 files", "English plural form must be correct");
    Expect(loc::ProgressStatus(loc::scan::ActiveVerb, 3, 13, L"Browser Caches") ==
               L"Scanning 3 of 13 · Browser Caches",
           "English progress status must be formatted correctly");
    Expect(loc::ScanItemResult(L"User Temp Files", L"1.0 MB", 4, 2) ==
               L"User Temp Files: 1.0 MB · 4 files · 2 files skipped",
           "English scan result must include a complete skipped-file phrase");

    localization::SetLanguage(localization::Language::Ukrainian);
    Expect(loc::ui::Scan.str() == L"Сканувати", "Ukrainian UI string must be selected");
    Expect(loc::Files(1) == L"1 файл", "Ukrainian singular form must be correct");
    Expect(loc::Files(2) == L"2 файли", "Ukrainian paucal form must be correct");
    Expect(loc::Files(5) == L"5 файлів", "Ukrainian plural form must be correct");
    Expect(loc::Files(11) == L"11 файлів", "Ukrainian teen plural form must be correct");
    Expect(loc::ScanItemResult(L"Тимчасові файли", L"1,0 МБ", 4, 2).find(L"пропущено 2 файли") !=
               std::wstring::npos,
           "Ukrainian scan result must include a complete skipped-file phrase");

    localization::SetLanguage(localization::Language::Russian);
    Expect(loc::ui::Scan.str() == L"Сканировать", "Russian UI string must be selected");
    Expect(loc::Files(1) == L"1 файл", "Russian singular form must be correct");
    Expect(loc::Files(2) == L"2 файла", "Russian paucal form must be correct");
    Expect(loc::Files(5) == L"5 файлов", "Russian plural form must be correct");
    Expect(loc::Files(11) == L"11 файлов", "Russian teen plural form must be correct");

    Expect(std::wstring(localization::LanguageCode(localization::Language::English)) == L"en",
           "English language code must be stable");
    Expect(std::wstring(localization::LanguageCode(localization::Language::Ukrainian)) == L"uk",
           "Ukrainian language code must be stable");
    Expect(std::wstring(localization::LanguageCode(localization::Language::Russian)) == L"ru",
           "Russian language code must be stable");

    localization::SetLanguage(original);
}

void TestSafeCleanupAndOfflineImageProtection() {
    const fs::path tempRoot = fs::temp_directory_path();
    TestDirectory testDirectory{
        tempRoot / (L"N91DiskCleaner_Regression_" + std::to_wstring(GetCurrentProcessId()))
    };
    std::error_code ec;
    fs::remove_all(testDirectory.path, ec);
    ec.clear();
    fs::create_directories(testDirectory.path, ec);
    if (ec) throw std::system_error(ec, "failed to create test directory");

    const fs::path oldFile = testDirectory.path / L"old.tmp";
    const fs::path recentFile = testDirectory.path / L"recent.tmp";
    WriteTestFile(oldFile, 4096);
    WriteTestFile(recentFile, 1024);
    fs::last_write_time(oldFile,
                        fs::file_time_type::clock::now() - std::chrono::hours(48), ec);
    if (ec) throw std::system_error(ec, "failed to age test file");

    const fs::path imageTree = testDirectory.path / L"mounted-image";
    fs::create_directories(imageTree / L"Windows" / L"System32");
    fs::create_directories(imageTree / L"Windows" / L"WinSxS");
    const fs::path imageMarker = imageTree / L"Windows" / L"System32" / L"protected.bin";
    WriteTestFile(imageMarker, 2048);

    Expect(LooksLikeOfflineWindowsImage(imageTree),
           "offline Windows tree signature must be detected");

    g_cancel.store(false);
    const ScanStats scan = ScanPath(testDirectory.path, true);
    Expect(scan.files == 1, "scan must include the old ordinary file only");
    Expect(scan.skipped == 2, "scan must skip the recent file and offline Windows tree");
    Expect(scan.errors == 0, "safe scan scenario must not report real errors");

    const CleanupStats cleanup = CleanupPath(testDirectory.path, true);
    Expect(cleanup.deletedFiles == 1, "cleanup must delete exactly the old ordinary file");
    Expect(cleanup.deletedBytes == 4096, "cleanup must report bytes from the deleted file");
    Expect(cleanup.skippedFiles == 2,
           "cleanup must count the recent file and protected image as skipped items");
    Expect(cleanup.errors == 0, "expected skips must not become real errors");
    Expect(CleanupIssueCount(cleanup, CleanupIssueKind::ProtectedOfflineImage) == 1,
           "protected offline image must be visible in the skip breakdown");
    Expect(!fs::exists(oldFile), "old ordinary file must be removed");
    Expect(fs::exists(recentFile), "recent file must remain");
    Expect(fs::exists(imageMarker), "offline Windows image contents must remain untouched");
}

} // namespace

int wmain() {
    try {
        TestIssueClassificationAndGrouping();
        TestRuntimeLocalization();
        TestSafeCleanupAndOfflineImageProtection();
    } catch (const std::exception& exception) {
        std::cerr << "FAIL: unexpected exception: " << exception.what() << '\n';
        return 2;
    }

    if (g_failures != 0) {
        std::cerr << g_failures << " regression check(s) failed.\n";
        return 1;
    }
    std::cout << "All cleanup regression checks passed.\n";
    return 0;
}
