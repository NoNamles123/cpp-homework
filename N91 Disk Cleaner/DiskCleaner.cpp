#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define NOMINMAX
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include <richedit.h>
#include <uxtheme.h>

#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cwctype>
#include <filesystem>
#include <iomanip>
#include <iterator>
#include <limits>
#include <mutex>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#include "Localization.h"

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "uxtheme.lib")

namespace fs = std::filesystem;
namespace loc = localization::english;

constexpr COLORREF COLOR_BG         = RGB(10, 14, 21);
constexpr COLORREF COLOR_PANEL      = RGB(17, 23, 33);
constexpr COLORREF COLOR_PANEL_2    = RGB(22, 30, 42);
constexpr COLORREF COLOR_CARD       = RGB(25, 34, 47);
constexpr COLORREF COLOR_CARD_HOVER = RGB(30, 41, 57);
constexpr COLORREF COLOR_BORDER     = RGB(43, 55, 72);
constexpr COLORREF COLOR_TEXT       = RGB(241, 245, 249);
constexpr COLORREF COLOR_MUTED      = RGB(148, 163, 184);
constexpr COLORREF COLOR_SUBTLE     = RGB(100, 116, 139);
constexpr COLORREF COLOR_ACCENT     = RGB(78, 128, 255);
constexpr COLORREF COLOR_ACCENT_2   = RGB(124, 92, 255);
constexpr COLORREF COLOR_GREEN      = RGB(52, 211, 153);
constexpr COLORREF COLOR_RED        = RGB(251, 113, 133);
constexpr COLORREF COLOR_YELLOW     = RGB(251, 191, 36);

constexpr UINT WM_APP_LOG          = WM_APP + 1;
constexpr UINT WM_APP_SIZE_RESULT  = WM_APP + 2;
constexpr UINT WM_APP_PROGRESS     = WM_APP + 3;
constexpr UINT WM_APP_FINISHED     = WM_APP + 4;
constexpr UINT WM_APP_STATUS       = WM_APP + 5;
constexpr UINT WM_APP_ACTIVE_ITEM  = WM_APP + 6;

constexpr int ID_SCAN               = 1001;
constexpr int ID_CLEAN              = 1002;
constexpr int ID_STOP               = 1003;
constexpr int ID_PRESET_RECOMMENDED = 1004;
constexpr int ID_PRESET_SAFE        = 1005;
constexpr int ID_PRESET_CLEAR       = 1006;
constexpr int ID_EXPORT             = 1007;
constexpr int ID_STORAGE_SETTINGS   = 1008;
constexpr int ID_CLEAR_LOG          = 1009;
constexpr int ID_PROTECT_RECENT     = 1010;
constexpr int ID_LANGUAGE           = 1011;
constexpr int ID_LANGUAGE_ENGLISH   = 1101;
constexpr int ID_LANGUAGE_UKRAINIAN = 1102;
constexpr int ID_LANGUAGE_RUSSIAN   = 1103;

enum class ActionType {
    UserTemp,
    WindowsTemp,
    WerReports,
    RecycleBin,
    DeliveryOptimization,
    WindowsUpdateCache,
    CrashDumps,
    ShaderCaches,
    BrowserCaches,
    CommunicationCaches,
    ThumbnailCaches,
    DismCleanup,
    DisableHibernation
};

enum class RiskLevel {
    Safe,
    Caution,
    System
};

enum class AppPhase {
    ReadyUnscanned,
    Scanning,
    ScanComplete,
    Cleaning,
    Cancelling,
    Completed,
    CompletedWithSkips,
    PartialFailure
};

enum class LogSeverity : WPARAM {
    Info,
    Success,
    Warning,
    Error
};

enum class CleanupIssueKind {
    RecentFile,
    AlreadyGone,
    BusyFile,
    AccessDenied,
    ReparsePoint,
    ProtectedOfflineImage,
    UnavailableEntry,
    CancelledOperation,
    UnsupportedEntry,
    RetainedDirectory,
    UnsafeTarget,
    MetadataFailure,
    EnumerationFailure,
    FileDeleteFailure,
    DirectoryDeleteFailure,
    ServiceFailure,
    SystemToolFailure,
    RecycleBinFailure,
    UnexpectedFailure,
    Count
};

constexpr size_t CLEANUP_LOG_LIMIT_PER_TYPE = 20;

struct CleanupIssueBucket {
    uint64_t total{};
    std::vector<std::wstring> samples;
};

struct ScanStats {
    uint64_t bytes{};
    uint64_t files{};
    uint64_t folders{};
    uint64_t skipped{};
    uint64_t errors{};
};

struct CleanupStats {
    uint64_t deletedBytes{};
    uint64_t deletedFiles{};
    uint64_t skippedFiles{};
    uint64_t errors{};
    std::array<CleanupIssueBucket, static_cast<size_t>(CleanupIssueKind::Count)> issues;
};

struct CleanerItem {
    ActionType action;
    std::wstring title;
    std::wstring description;
    bool defaultChecked;
    RiskLevel risk;
    HWND checkbox{};
    ScanStats scan{};
    bool scanned{};
    bool hovered{};
};

struct SizeResult {
    int index{};
    ScanStats stats{};
    uint64_t session{};
};

struct FinishResult {
    CleanupStats cleanup{};
    uint64_t freeSpaceDelta{};
    bool freeSpaceMeasured{};
    bool cancelled{};
    uint64_t session{};
};

struct OperationTask {
    int index{};
    ActionType action{};
    std::wstring title;
    RiskLevel risk{};
    uint64_t estimatedBytes{};
    uint64_t estimatedFiles{};
};

struct DriveStats {
    uint64_t total{};
    uint64_t free{};
    bool valid{};
};

struct NativePathAttributes {
    bool available{};
    bool reparsePoint{};
    bool directory{};
    std::error_code error;
};

static HINSTANCE g_instance{};
static HWND g_main{};
static HWND g_title{};
static HWND g_subtitle{};
static HWND g_freeCaption{};
static HWND g_log{};
static HWND g_progress{};
static HWND g_scanButton{};
static HWND g_cleanButton{};
static HWND g_stopButton{};
static HWND g_recommendedButton{};
static HWND g_safeButton{};
static HWND g_clearSelectionButton{};
static HWND g_exportButton{};
static HWND g_storageButton{};
static HWND g_languageButton{};
static HWND g_clearLogButton{};
static HWND g_protectRecentCheck{};
static HWND g_freeValue{};
static HWND g_freeDetail{};
static HWND g_foundCaption{};
static HWND g_foundValue{};
static HWND g_foundDetail{};
static HWND g_selectedCaption{};
static HWND g_selectedValue{};
static HWND g_selectedDetail{};
static HWND g_itemsTitle{};
static HWND g_itemsHint{};
static HWND g_logTitle{};
static HWND g_logHint{};
static HWND g_status{};
static HWND g_statusDetail{};
static HFONT g_fontHero{};
static HFONT g_fontTitle{};
static HFONT g_fontSubtitle{};
static HFONT g_fontValue{};
static HFONT g_fontNormal{};
static HFONT g_fontSmall{};
static HFONT g_fontBold{};
static HFONT g_fontTiny{};
static HFONT g_fontMono{};
static HBRUSH g_bgBrush{};
static HBRUSH g_panelBrush{};
static std::vector<CleanerItem> g_items;
static std::atomic_bool g_busy{false};
static std::atomic_bool g_cancel{false};
static std::atomic_bool g_protectRecent{true};
static std::atomic_uint64_t g_sessionCounter{0};
static uint64_t g_currentSession{};
static AppPhase g_phase{AppPhase::ReadyUnscanned};
static bool g_hasScanned{};
static bool g_closeWhenDone{};
static int g_activeItem{-1};
static UINT g_dpi{96};
static uint64_t g_lastFreedBytes{};
static bool g_lastFreedBytesMeasured{};
static CleanupStats g_lastCleanupStats{};
static bool g_hasLastCleanup{};
static bool g_showCleanupSummary{};
static RECT g_freeCard{};
static RECT g_foundCard{};
static RECT g_selectedCard{};
static RECT g_itemsPanel{};
static RECT g_logPanel{};
static RECT g_footerRect{};

std::wstring Env(const wchar_t* name) {
    wchar_t buffer[32768]{};
    DWORD len = GetEnvironmentVariableW(name, buffer, static_cast<DWORD>(std::size(buffer)));
    if (len == 0 || len >= std::size(buffer)) return L"";
    return std::wstring(buffer, len);
}

std::wstring WindowsDir() {
    wchar_t buffer[MAX_PATH]{};
    UINT len = GetWindowsDirectoryW(buffer, MAX_PATH);
    return len ? std::wstring(buffer, len) : L"C:\\Windows";
}

wchar_t SystemDriveLetter() {
    const std::wstring win = WindowsDir();
    return (win.size() >= 2 && win[1] == L':') ? win[0] : L'C';
}

std::wstring SystemDriveRoot() {
    std::wstring root;
    root += SystemDriveLetter();
    root += L":\\";
    return root;
}

DriveStats QueryDriveStats() {
    ULARGE_INTEGER freeAvailable{}, total{}, freeTotal{};
    if (!GetDiskFreeSpaceExW(SystemDriveRoot().c_str(), &freeAvailable, &total, &freeTotal)) return {};
    return {total.QuadPart, freeTotal.QuadPart, true};
}

std::wstring FormatBytes(uint64_t bytes) {
    double value = static_cast<double>(bytes);
    size_t unit = 0;
    while (value >= 1024.0 && unit < 4) {
        value /= 1024.0;
        ++unit;
    }
    std::wostringstream out;
    if (unit >= 3) out << std::fixed << std::setprecision(2);
    else if (unit == 2) out << std::fixed << std::setprecision(1);
    else out << std::fixed << std::setprecision(0);
    out << value << L" " << loc::format::ByteUnits[unit];
    return out.str();
}

std::wstring FormatMeasuredBytes(uint64_t bytes, bool measured) {
    return measured ? FormatBytes(bytes) : loc::format::NotAvailable.str();
}

void PostLog(const std::wstring& text, LogSeverity severity = LogSeverity::Info) {
    if (!IsWindow(g_main)) return;
    auto* copy = new std::wstring(text);
    if (!PostMessageW(g_main, WM_APP_LOG, static_cast<WPARAM>(severity), reinterpret_cast<LPARAM>(copy))) delete copy;
}

void PostLog(const localization::LocalizedString& text, LogSeverity severity = LogSeverity::Info) {
    PostLog(text.str(), severity);
}

void PostStatus(const std::wstring& text) {
    if (!IsWindow(g_main)) return;
    auto* copy = new std::wstring(text);
    if (!PostMessageW(g_main, WM_APP_STATUS, 0, reinterpret_cast<LPARAM>(copy))) delete copy;
}

void PostStatus(const localization::LocalizedString& text) {
    PostStatus(text.str());
}

template <typename T>
void PostOwned(UINT message, WPARAM wParam, T* payload) {
    if (!IsWindow(g_main) || !PostMessageW(g_main, message, wParam, reinterpret_cast<LPARAM>(payload))) {
        delete payload;
    }
}

std::wstring LowerCopy(std::wstring value) {
    std::transform(value.begin(), value.end(), value.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(std::towlower(ch));
    });
    return value;
}

std::wstring PathKey(const fs::path& path) {
    std::wstring value = path.lexically_normal().wstring();
    while (value.size() > 3 && (value.back() == L'\\' || value.back() == L'/')) value.pop_back();
    return LowerCopy(value);
}

bool IsSafeCleanupTarget(const fs::path& path) {
    if (path.empty() || !path.is_absolute()) return false;

    const std::wstring key = PathKey(path);
    if (key.size() < 4 || key == PathKey(path.root_path())) return false;

    std::vector<fs::path> blockedRoots = {
        fs::path(SystemDriveRoot()),
        fs::path(WindowsDir())
    };
    for (const wchar_t* name : {L"USERPROFILE", L"LOCALAPPDATA", L"APPDATA", L"PROGRAMDATA"}) {
        const auto value = Env(name);
        if (!value.empty()) blockedRoots.emplace_back(value);
    }

    for (const auto& blocked : blockedRoots) {
        if (key == PathKey(blocked)) return false;
    }
    return true;
}

void AddBytesClamped(uint64_t& total, uintmax_t value) {
    if (value > (std::numeric_limits<uint64_t>::max)() - total) {
        total = (std::numeric_limits<uint64_t>::max)();
    } else {
        total += static_cast<uint64_t>(value);
    }
}

NativePathAttributes QueryNativePathAttributes(const fs::path& path) {
    NativePathAttributes result;
    const DWORD attributes = GetFileAttributesW(path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        result.error = std::error_code(static_cast<int>(GetLastError()), std::system_category());
        return result;
    }
    result.available = true;
    result.reparsePoint = (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
    result.directory = (attributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
    return result;
}

bool LooksLikeOfflineWindowsImage(const fs::path& path) {
    const auto windows = QueryNativePathAttributes(path / L"Windows");
    if (!windows.available || !windows.directory) return false;
    if (windows.reparsePoint) return true;

    const auto system32 = QueryNativePathAttributes(path / L"Windows" / L"System32");
    const auto componentStore = QueryNativePathAttributes(path / L"Windows" / L"WinSxS");

    const auto definitelyMissing = [](const NativePathAttributes& attributes) {
        if (attributes.available || attributes.error.category() != std::system_category()) return false;
        const DWORD code = static_cast<DWORD>(attributes.error.value());
        return code == ERROR_FILE_NOT_FOUND || code == ERROR_PATH_NOT_FOUND;
    };
    if ((!system32.available && !definitelyMissing(system32)) ||
        (!componentStore.available && !definitelyMissing(componentStore))) {
        return true;
    }

    const bool hasSystem32 = system32.available && system32.directory;
    const bool hasComponentStore = componentStore.available && componentStore.directory;
    return hasSystem32 || hasComponentStore;
}

size_t CleanupIssueIndex(CleanupIssueKind kind) {
    return static_cast<size_t>(kind);
}

bool IsRealCleanupError(CleanupIssueKind kind) {
    switch (kind) {
        case CleanupIssueKind::RecentFile:
        case CleanupIssueKind::AlreadyGone:
        case CleanupIssueKind::BusyFile:
        case CleanupIssueKind::AccessDenied:
        case CleanupIssueKind::ReparsePoint:
        case CleanupIssueKind::ProtectedOfflineImage:
        case CleanupIssueKind::UnavailableEntry:
        case CleanupIssueKind::CancelledOperation:
        case CleanupIssueKind::UnsupportedEntry:
        case CleanupIssueKind::RetainedDirectory:
            return false;
        default:
            return true;
    }
}

bool IsSkippedFileOutcome(CleanupIssueKind kind) {
    switch (kind) {
        case CleanupIssueKind::RecentFile:
        case CleanupIssueKind::AlreadyGone:
        case CleanupIssueKind::BusyFile:
        case CleanupIssueKind::AccessDenied:
        case CleanupIssueKind::ReparsePoint:
        case CleanupIssueKind::ProtectedOfflineImage:
        case CleanupIssueKind::UnavailableEntry:
        case CleanupIssueKind::UnsupportedEntry:
            return true;
        default:
            return false;
    }
}

std::wstring CleanupIssueLabel(CleanupIssueKind kind) {
    switch (kind) {
        case CleanupIssueKind::RecentFile:             return loc::issue::RecentFile.str();
        case CleanupIssueKind::AlreadyGone:            return loc::issue::AlreadyGone.str();
        case CleanupIssueKind::BusyFile:               return loc::issue::BusyFile.str();
        case CleanupIssueKind::AccessDenied:           return loc::issue::AccessDenied.str();
        case CleanupIssueKind::ReparsePoint:           return loc::issue::ReparsePoint.str();
        case CleanupIssueKind::ProtectedOfflineImage:  return loc::issue::ProtectedOfflineImage.str();
        case CleanupIssueKind::UnavailableEntry:       return loc::issue::UnavailableEntry.str();
        case CleanupIssueKind::CancelledOperation:     return loc::issue::CancelledOperation.str();
        case CleanupIssueKind::UnsupportedEntry:       return loc::issue::UnsupportedEntry.str();
        case CleanupIssueKind::RetainedDirectory:      return loc::issue::RetainedDirectory.str();
        case CleanupIssueKind::UnsafeTarget:           return loc::issue::UnsafeTarget.str();
        case CleanupIssueKind::MetadataFailure:        return loc::issue::MetadataFailure.str();
        case CleanupIssueKind::EnumerationFailure:     return loc::issue::EnumerationFailure.str();
        case CleanupIssueKind::FileDeleteFailure:      return loc::issue::FileDeleteFailure.str();
        case CleanupIssueKind::DirectoryDeleteFailure: return loc::issue::DirectoryDeleteFailure.str();
        case CleanupIssueKind::ServiceFailure:         return loc::issue::ServiceFailure.str();
        case CleanupIssueKind::SystemToolFailure:      return loc::issue::SystemToolFailure.str();
        case CleanupIssueKind::RecycleBinFailure:      return loc::issue::RecycleBinFailure.str();
        case CleanupIssueKind::UnexpectedFailure:      return loc::issue::UnexpectedFailure.str();
        case CleanupIssueKind::Count:                  break;
    }
    return loc::issue::Unknown.str();
}

CleanupIssueKind ClassifyFilesystemIssue(const std::error_code& ec, CleanupIssueKind fallback) {
    if (!ec) return CleanupIssueKind::AlreadyGone;

    if (ec.category() == std::system_category()) {
        switch (static_cast<DWORD>(ec.value())) {
            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
            case ERROR_DELETE_PENDING:
                return CleanupIssueKind::AlreadyGone;
            case ERROR_SHARING_VIOLATION:
            case ERROR_LOCK_VIOLATION:
            case ERROR_USER_MAPPED_FILE:
            case ERROR_PATH_BUSY:
            case ERROR_BUSY:
            case ERROR_BUSY_DRIVE:
            case ERROR_OPEN_FILES:
            case ERROR_DEVICE_IN_USE:
            case ERROR_LOCKED:
            case ERROR_LOCK_FAILED:
                return CleanupIssueKind::BusyFile;
            case ERROR_ACCESS_DENIED:
            case ERROR_PRIVILEGE_NOT_HELD:
            case ERROR_WRITE_PROTECT:
            case ERROR_NETWORK_ACCESS_DENIED:
            case ERROR_NOACCESS:
            case ERROR_CANNOT_MAKE:
            case ERROR_INVALID_ACCESS:
                return CleanupIssueKind::AccessDenied;
            case ERROR_CANT_ACCESS_FILE:
                return CleanupIssueKind::UnavailableEntry;
            case ERROR_OPERATION_ABORTED:
            case ERROR_CANCELLED:
                return CleanupIssueKind::CancelledOperation;
            case ERROR_DIR_NOT_EMPTY:
                return CleanupIssueKind::RetainedDirectory;
            default:
                break;
        }
    }

    if (ec == std::errc::no_such_file_or_directory) return CleanupIssueKind::AlreadyGone;
    if (ec == std::errc::device_or_resource_busy || ec == std::errc::no_lock_available)
        return CleanupIssueKind::BusyFile;
    if (ec == std::errc::permission_denied) return CleanupIssueKind::AccessDenied;
    if (ec == std::errc::operation_canceled) return CleanupIssueKind::CancelledOperation;
    if (ec == std::errc::directory_not_empty) return CleanupIssueKind::RetainedDirectory;
    return fallback;
}

void RecordCleanupIssue(CleanupStats& stats, CleanupIssueKind kind, bool skippedFile,
                        const std::wstring& subject = L"", int64_t nativeCode = 0) {
    if (skippedFile || IsSkippedFileOutcome(kind)) ++stats.skippedFiles;

    auto& bucket = stats.issues[CleanupIssueIndex(kind)];
    if (bucket.total == 0 && IsRealCleanupError(kind)) ++stats.errors;
    ++bucket.total;

    if (!IsRealCleanupError(kind) || bucket.samples.size() >= CLEANUP_LOG_LIMIT_PER_TYPE) return;

    std::wstring sample = loc::issue::ErrorPrefix + CleanupIssueLabel(kind);
    if (!subject.empty()) sample += L" — " + subject;
    if (nativeCode != 0) sample += loc::issue::CodePrefix + std::to_wstring(nativeCode) + loc::issue::CodeSuffix;
    bucket.samples.push_back(std::move(sample));
}

void RecordCleanupIssue(CleanupStats& stats, CleanupIssueKind kind, bool skippedFile,
                        const localization::LocalizedString& subject, int64_t nativeCode = 0) {
    RecordCleanupIssue(stats, kind, skippedFile, subject.str(), nativeCode);
}

void RecordFilesystemIssue(CleanupStats& stats, CleanupIssueKind fallback, const fs::path& path,
                           const std::error_code& ec, bool skippedFile) {
    const auto kind = ClassifyFilesystemIssue(ec, fallback);
    RecordCleanupIssue(stats, kind, skippedFile, path.wstring(), static_cast<int64_t>(ec.value()));
}

void RecordScanFilesystemIssue(ScanStats& stats, CleanupIssueKind fallback,
                               const std::error_code& ec) {
    const CleanupIssueKind kind = ClassifyFilesystemIssue(ec, fallback);
    ++stats.skipped;
    if (IsRealCleanupError(kind)) ++stats.errors;
}

void MergeCleanupStatsSameCategory(CleanupStats& target, const CleanupStats& source) {
    AddBytesClamped(target.deletedBytes, source.deletedBytes);
    target.deletedFiles += source.deletedFiles;
    target.skippedFiles += source.skippedFiles;

    for (size_t index = 0; index < source.issues.size(); ++index) {
        const auto kind = static_cast<CleanupIssueKind>(index);
        const auto& from = source.issues[index];
        if (from.total == 0) continue;

        auto& to = target.issues[index];
        if (to.total == 0 && IsRealCleanupError(kind)) ++target.errors;
        to.total += from.total;
        for (const auto& sample : from.samples) {
            if (to.samples.size() >= CLEANUP_LOG_LIMIT_PER_TYPE) break;
            to.samples.push_back(sample);
        }
    }
}

void AccumulateCleanupCategory(CleanupStats& total, const CleanupStats& category,
                               const std::wstring& categoryTitle) {
    AddBytesClamped(total.deletedBytes, category.deletedBytes);
    total.deletedFiles += category.deletedFiles;
    total.skippedFiles += category.skippedFiles;
    total.errors += category.errors;

    for (size_t index = 0; index < category.issues.size(); ++index) {
        const auto& from = category.issues[index];
        if (from.total == 0) continue;

        auto& to = total.issues[index];
        to.total += from.total;
        for (const auto& sample : from.samples) {
            if (to.samples.size() >= CLEANUP_LOG_LIMIT_PER_TYPE) break;
            to.samples.push_back(L"[" + categoryTitle + L"] " + sample);
        }
    }
}

void AccumulateCleanupCategory(CleanupStats& total, const CleanupStats& category,
                               const localization::LocalizedString& categoryTitle) {
    AccumulateCleanupCategory(total, category, categoryTitle.str());
}

uint64_t CleanupIssueCount(const CleanupStats& stats, CleanupIssueKind kind) {
    return stats.issues[CleanupIssueIndex(kind)].total;
}

std::wstring CleanupSkipBreakdown(const CleanupStats& stats) {
    struct Part { CleanupIssueKind kind; const wchar_t* label; };
    const Part parts[] = {
        {CleanupIssueKind::RecentFile,         loc::skip::Recent},
        {CleanupIssueKind::BusyFile,           loc::skip::Busy},
        {CleanupIssueKind::AccessDenied,       loc::skip::AccessDenied},
        {CleanupIssueKind::AlreadyGone,        loc::skip::AlreadyGone},
        {CleanupIssueKind::ReparsePoint,       loc::skip::ReparsePoint},
        {CleanupIssueKind::ProtectedOfflineImage, loc::skip::ProtectedOfflineImage},
        {CleanupIssueKind::UnavailableEntry,   loc::skip::UnavailableEntry},
        {CleanupIssueKind::UnsupportedEntry,   loc::skip::Other},
        {CleanupIssueKind::CancelledOperation, loc::skip::Cancelled}
    };

    std::wostringstream out;
    bool first = true;
    for (const auto& part : parts) {
        const uint64_t count = CleanupIssueCount(stats, part.kind);
        if (count == 0) continue;
        if (!first) out << L", ";
        out << part.label << L" " << count;
        first = false;
    }
    return out.str();
}

void LogCleanupDiagnostics(const CleanupStats& stats) {
    for (size_t index = 0; index < stats.issues.size(); ++index) {
        const auto kind = static_cast<CleanupIssueKind>(index);
        if (!IsRealCleanupError(kind)) continue;

        const auto& bucket = stats.issues[index];
        for (const auto& sample : bucket.samples) PostLog(sample, LogSeverity::Error);
        if (bucket.total > bucket.samples.size()) {
            const uint64_t hidden = bucket.total - bucket.samples.size();
            PostLog(loc::HiddenMessages(hidden, CleanupIssueLabel(kind)), LogSeverity::Error);
        }
    }
}

bool IncludeFileByAge(const fs::path& path, bool protectRecent, ScanStats& stats) {
    if (!protectRecent) return true;
    std::error_code ec;
    const auto writeTime = fs::last_write_time(path, ec);
    if (ec) {
        RecordScanFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, ec);
        return false;
    }
    const auto cutoff = fs::file_time_type::clock::now() - std::chrono::hours(24);
    if (writeTime > cutoff) {
        ++stats.skipped;
        return false;
    }
    return true;
}

void ScanRegularFile(const fs::path& path, bool protectRecent, ScanStats& stats) {
    if (!IncludeFileByAge(path, protectRecent, stats)) return;
    std::error_code ec;
    const auto size = fs::file_size(path, ec);
    if (ec) {
        RecordScanFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, ec);
        return;
    }
    AddBytesClamped(stats.bytes, size);
    ++stats.files;
}

ScanStats ScanPath(const fs::path& path, bool protectRecent) {
    ScanStats stats;
    if (!IsSafeCleanupTarget(path)) {
        stats.errors = 1;
        return stats;
    }

    const NativePathAttributes rootAttributes = QueryNativePathAttributes(path);
    if (!rootAttributes.available) {
        RecordScanFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, rootAttributes.error);
        return stats;
    }
    if (rootAttributes.reparsePoint) {
        ++stats.skipped;
        return stats;
    }
    if (rootAttributes.directory && LooksLikeOfflineWindowsImage(path)) {
        ++stats.skipped;
        return stats;
    }

    std::error_code ec;
    if (fs::is_symlink(fs::symlink_status(path, ec))) {
        ++stats.skipped;
        return stats;
    }
    if (fs::is_regular_file(path, ec)) {
        ScanRegularFile(path, protectRecent, stats);
        return stats;
    }
    if (!fs::is_directory(path, ec)) {
        if (ec) ++stats.errors;
        return stats;
    }

    fs::recursive_directory_iterator it(path, fs::directory_options::skip_permission_denied, ec), end;
    if (ec) {
        ++stats.errors;
        ec.clear();
    }
    while (it != end && !g_cancel.load()) {
        const auto entryPath = it->path();
        const NativePathAttributes attributes = QueryNativePathAttributes(entryPath);
        if (!attributes.available) {
            RecordScanFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, attributes.error);
        } else if (attributes.reparsePoint) {
            ++stats.skipped;
            if (attributes.directory) it.disable_recursion_pending();
        } else if (attributes.directory && LooksLikeOfflineWindowsImage(entryPath)) {
            ++stats.skipped;
            it.disable_recursion_pending();
        } else {
            std::error_code entryEc;
            const auto status = it->symlink_status(entryEc);
            if (entryEc) {
                RecordScanFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, entryEc);
            } else if (fs::is_symlink(status)) {
                ++stats.skipped;
                if (fs::is_directory(status)) it.disable_recursion_pending();
            } else if (fs::is_regular_file(status)) {
                ScanRegularFile(entryPath, protectRecent, stats);
            } else if (fs::is_directory(status)) {
                ++stats.folders;
            }
        }

        it.increment(ec);
        if (ec) {
            ++stats.errors;
            ec.clear();
        }
    }
    return stats;
}

ScanStats ScanPaths(const std::vector<fs::path>& paths, bool protectRecent) {
    ScanStats total;
    for (const auto& path : paths) {
        if (g_cancel.load()) break;
        const auto part = ScanPath(path, protectRecent);
        AddBytesClamped(total.bytes, part.bytes);
        total.files += part.files;
        total.folders += part.folders;
        total.skipped += part.skipped;
        if (part.errors > 0) total.errors = 1;
    }
    return total;
}

bool IncludeFileByAgeForCleanup(const fs::path& path, bool protectRecent, CleanupStats& stats) {
    if (!protectRecent) return true;

    std::error_code ec;
    const auto writeTime = fs::last_write_time(path, ec);
    if (ec) {
        RecordFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, path, ec, true);
        return false;
    }

    const auto cutoff = fs::file_time_type::clock::now() - std::chrono::hours(24);
    if (writeTime > cutoff) {
        RecordCleanupIssue(stats, CleanupIssueKind::RecentFile, true, path.wstring());
        return false;
    }
    return true;
}

void RemoveOneFile(const fs::path& path, bool protectRecent, CleanupStats& stats) {
    if (!IncludeFileByAgeForCleanup(path, protectRecent, stats)) return;

    std::error_code sizeEc;
    const auto size = fs::file_size(path, sizeEc);
    std::error_code removeEc;
    const bool removed = fs::remove(path, removeEc);
    if (removed) {
        if (!sizeEc) AddBytesClamped(stats.deletedBytes, size);
        ++stats.deletedFiles;
        return;
    }

    if (removeEc) {
        RecordFilesystemIssue(stats, CleanupIssueKind::FileDeleteFailure, path, removeEc, true);
    } else {
        RecordCleanupIssue(stats, CleanupIssueKind::AlreadyGone, true, path.wstring());
    }
}

CleanupStats CleanupPath(const fs::path& path, bool protectRecent) {
    CleanupStats stats;
    if (!IsSafeCleanupTarget(path)) {
        RecordCleanupIssue(stats, CleanupIssueKind::UnsafeTarget, false, path.wstring());
        return stats;
    }

    const NativePathAttributes rootAttributes = QueryNativePathAttributes(path);
    if (!rootAttributes.available) {
        const auto kind = ClassifyFilesystemIssue(rootAttributes.error, CleanupIssueKind::MetadataFailure);
        RecordCleanupIssue(stats, kind, true, path.wstring(),
                           static_cast<int64_t>(rootAttributes.error.value()));
        return stats;
    }
    if (rootAttributes.reparsePoint) {
        RecordCleanupIssue(stats, CleanupIssueKind::ReparsePoint, true, path.wstring());
        return stats;
    }
    if (rootAttributes.directory && LooksLikeOfflineWindowsImage(path)) {
        RecordCleanupIssue(stats, CleanupIssueKind::ProtectedOfflineImage, false, path.wstring());
        return stats;
    }

    std::error_code ec;
    const auto rootStatus = fs::symlink_status(path, ec);
    if (ec) {
        RecordFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, path, ec, true);
        return stats;
    }
    if (!fs::exists(rootStatus)) {
        RecordCleanupIssue(stats, CleanupIssueKind::AlreadyGone, true, path.wstring());
        return stats;
    }
    if (fs::is_symlink(rootStatus)) {
        RecordCleanupIssue(stats, CleanupIssueKind::ReparsePoint, true, path.wstring());
        return stats;
    }
    if (fs::is_regular_file(rootStatus)) {
        RemoveOneFile(path, protectRecent, stats);
        return stats;
    }
    if (!fs::is_directory(rootStatus)) {
        RecordCleanupIssue(stats, CleanupIssueKind::UnsupportedEntry, true, path.wstring());
        return stats;
    }

    std::vector<fs::path> directories;
    std::vector<fs::path> pendingDirectories{path};
    while (!pendingDirectories.empty() && !g_cancel.load()) {
        fs::path currentDirectory = std::move(pendingDirectories.back());
        pendingDirectories.pop_back();

        std::error_code iteratorEc;
        fs::directory_iterator it(currentDirectory, fs::directory_options::none, iteratorEc), end;
        if (iteratorEc) {
            RecordFilesystemIssue(stats, CleanupIssueKind::EnumerationFailure,
                                  currentDirectory, iteratorEc, true);
            continue;
        }

        while (it != end && !g_cancel.load()) {
            const auto entryPath = it->path();
            const NativePathAttributes attributes = QueryNativePathAttributes(entryPath);
            if (!attributes.available) {
                RecordFilesystemIssue(stats, CleanupIssueKind::MetadataFailure,
                                      entryPath, attributes.error, true);
            } else if (attributes.reparsePoint) {
                RecordCleanupIssue(stats, CleanupIssueKind::ReparsePoint, true, entryPath.wstring());
            } else if (attributes.directory && LooksLikeOfflineWindowsImage(entryPath)) {
                RecordCleanupIssue(stats, CleanupIssueKind::ProtectedOfflineImage, false,
                                   entryPath.wstring());
            } else {
                std::error_code entryEc;
                const auto status = it->symlink_status(entryEc);
                if (entryEc) {
                    RecordFilesystemIssue(stats, CleanupIssueKind::MetadataFailure, entryPath, entryEc, true);
                } else if (!fs::exists(status)) {
                    RecordCleanupIssue(stats, CleanupIssueKind::AlreadyGone, true, entryPath.wstring());
                } else if (fs::is_symlink(status)) {
                    RecordCleanupIssue(stats, CleanupIssueKind::ReparsePoint, true, entryPath.wstring());
                } else if (fs::is_regular_file(status)) {
                    RemoveOneFile(entryPath, protectRecent, stats);
                } else if (fs::is_directory(status)) {
                    pendingDirectories.push_back(entryPath);
                    directories.push_back(entryPath);
                } else {
                    RecordCleanupIssue(stats, CleanupIssueKind::UnsupportedEntry, true, entryPath.wstring());
                }
            }

            it.increment(iteratorEc);
            if (iteratorEc) {
                RecordFilesystemIssue(stats, CleanupIssueKind::EnumerationFailure,
                                      currentDirectory, iteratorEc, true);
                break;
            }
        }
    }

    std::sort(directories.begin(), directories.end(), [](const fs::path& left, const fs::path& right) {
        return left.native().size() > right.native().size();
    });
    for (const auto& directory : directories) {
        if (g_cancel.load()) break;
        std::error_code removeEc;
        const bool removed = fs::remove(directory, removeEc);
        if (!removed) {
            const auto kind = removeEc
                ? ClassifyFilesystemIssue(removeEc, CleanupIssueKind::DirectoryDeleteFailure)
                : CleanupIssueKind::AlreadyGone;
            RecordCleanupIssue(stats, kind, false, directory.wstring(),
                               static_cast<int64_t>(removeEc.value()));
        }
    }
    return stats;
}

CleanupStats CleanupPaths(const std::vector<fs::path>& paths, bool protectRecent) {
    CleanupStats total;
    for (const auto& path : paths) {
        if (g_cancel.load()) break;
        const auto part = CleanupPath(path, protectRecent);
        MergeCleanupStatsSameCategory(total, part);
    }
    return total;
}

std::vector<fs::path> UserTempPaths() {
    std::vector<fs::path> paths;
    wchar_t tmp[MAX_PATH]{};
    DWORD len = GetTempPathW(MAX_PATH, tmp);
    if (len > 0 && len < MAX_PATH) paths.emplace_back(tmp);
    return paths;
}

std::vector<fs::path> WindowsTempPaths() {
    return {fs::path(WindowsDir()) / L"Temp"};
}

std::vector<fs::path> WerPaths() {
    const auto programData = Env(L"PROGRAMDATA");
    if (programData.empty()) return {};
    return {fs::path(programData) / L"Microsoft\\Windows\\WER\\ReportArchive",
            fs::path(programData) / L"Microsoft\\Windows\\WER\\ReportQueue",
            fs::path(programData) / L"Microsoft\\Windows\\WER\\Temp"};
}

std::vector<fs::path> DeliveryOptimizationPaths() {
    const auto programData = Env(L"PROGRAMDATA");
    std::vector<fs::path> paths;
    if (!programData.empty()) paths.push_back(fs::path(programData) / L"Microsoft\\Windows\\DeliveryOptimization\\Cache");
    paths.push_back(fs::path(WindowsDir()) / L"ServiceProfiles\\NetworkService\\AppData\\Local\\Microsoft\\Windows\\DeliveryOptimization\\Cache");
    return paths;
}

std::vector<fs::path> WindowsUpdatePaths() {
    return {
        fs::path(WindowsDir()) / L"SoftwareDistribution\\Download"
    };
}

std::vector<fs::path> CrashDumpPaths() {
    const auto local = Env(L"LOCALAPPDATA");
    std::vector<fs::path> paths = {fs::path(WindowsDir()) / L"MEMORY.DMP",
                                   fs::path(WindowsDir()) / L"Minidump",
                                   fs::path(WindowsDir()) / L"LiveKernelReports"};
    if (!local.empty()) paths.push_back(fs::path(local) / L"CrashDumps");
    return paths;
}

std::vector<fs::path> ShaderCachePaths() {
    const auto local = Env(L"LOCALAPPDATA");
    const auto programData = Env(L"PROGRAMDATA");
    std::vector<fs::path> paths;
    if (!local.empty()) {
        const fs::path base(local);
        paths.push_back(base / L"D3DSCache");
        paths.push_back(base / L"NVIDIA\\DXCache");
        paths.push_back(base / L"NVIDIA\\GLCache");
        paths.push_back(base / L"AMD\\DxCache");
        paths.push_back(base / L"AMD\\GLCache");
    }
    if (!programData.empty()) paths.push_back(fs::path(programData) / L"NVIDIA Corporation\\NV_Cache");
    return paths;
}

void AddCacheTriplet(std::vector<fs::path>& paths, const fs::path& profile) {
    paths.push_back(profile / L"Cache");
    paths.push_back(profile / L"Code Cache");
    paths.push_back(profile / L"GPUCache");
}

void AddBrowserProfileCaches(std::vector<fs::path>& paths, const fs::path& userDataRoot) {
    std::error_code ec;
    if (!fs::exists(userDataRoot, ec)) return;

    for (fs::directory_iterator it(userDataRoot, fs::directory_options::skip_permission_denied, ec), end;
         it != end;
         it.increment(ec)) {
        if (ec) {
            ec.clear();
            continue;
        }
        if (!it->is_directory(ec)) continue;

        const auto name = it->path().filename().wstring();
        if (name == L"Default" || name == L"Guest Profile" || name == L"System Profile" || name.rfind(L"Profile ", 0) == 0) {
            AddCacheTriplet(paths, it->path());
        }
    }
}

std::vector<fs::path> BrowserCachePaths() {
    const auto local = Env(L"LOCALAPPDATA");
    const auto roaming = Env(L"APPDATA");
    std::vector<fs::path> paths;
    if (!local.empty()) {
        const fs::path base(local);
        AddBrowserProfileCaches(paths, base / L"Google\\Chrome\\User Data");
        AddBrowserProfileCaches(paths, base / L"Microsoft\\Edge\\User Data");
        AddBrowserProfileCaches(paths, base / L"BraveSoftware\\Brave-Browser\\User Data");
        AddBrowserProfileCaches(paths, base / L"Vivaldi\\User Data");

        const fs::path operaGx = base / L"Opera Software\\Opera GX Stable";
        AddCacheTriplet(paths, operaGx);

        const fs::path firefoxProfiles = base / L"Mozilla\\Firefox\\Profiles";
        std::error_code ec;
        if (fs::exists(firefoxProfiles, ec)) {
            for (fs::directory_iterator it(firefoxProfiles, fs::directory_options::skip_permission_denied, ec), end;
                 it != end; it.increment(ec)) {
                if (ec) { ec.clear(); continue; }
                if (it->is_directory(ec)) paths.push_back(it->path() / L"cache2");
            }
        }
    }
    if (!roaming.empty()) {
        AddCacheTriplet(paths, fs::path(roaming) / L"Opera Software\\Opera GX Stable");
    }
    return paths;
}

std::vector<fs::path> CommunicationCachePaths() {
    const auto local = Env(L"LOCALAPPDATA");
    const auto roaming = Env(L"APPDATA");
    std::vector<fs::path> paths;
    if (!roaming.empty()) {
        const fs::path base(roaming);
        for (const wchar_t* app : {L"discord", L"discordcanary", L"discordptb", L"Slack"}) {
            AddCacheTriplet(paths, base / app);
        }
        const fs::path teams = base / L"Microsoft\\Teams";
        AddCacheTriplet(paths, teams);
        paths.push_back(teams / L"blob_storage");
        paths.push_back(teams / L"databases");
    }
    if (!local.empty()) {
        const fs::path teams = fs::path(local) / L"Packages\\MSTeams_8wekyb3d8bbwe\\LocalCache\\Microsoft\\MSTeams";
        AddCacheTriplet(paths, teams / L"EBWebView\\Default");
    }
    return paths;
}

std::vector<fs::path> ThumbnailCachePaths() {
    const auto local = Env(L"LOCALAPPDATA");
    if (local.empty()) return {};
    const fs::path explorer = fs::path(local) / L"Microsoft\\Windows\\Explorer";
    std::vector<fs::path> paths;
    std::error_code ec;
    if (!fs::exists(explorer, ec)) return paths;
    for (fs::directory_iterator it(explorer, fs::directory_options::skip_permission_denied, ec), end;
         it != end; it.increment(ec)) {
        if (ec) { ec.clear(); continue; }
        if (!it->is_regular_file(ec)) continue;
        const auto name = LowerCopy(it->path().filename().wstring());
        if ((name.rfind(L"thumbcache_", 0) == 0 || name.rfind(L"iconcache_", 0) == 0) &&
            it->path().extension() == L".db") {
            paths.push_back(it->path());
        }
    }
    return paths;
}

std::vector<fs::path> RecycleBinPaths() {
    return {fs::path(SystemDriveRoot()) / L"$Recycle.Bin"};
}

std::vector<fs::path> HibernationPaths() {
    return {fs::path(SystemDriveRoot()) / L"hiberfil.sys"};
}

std::vector<fs::path> PathsForAction(ActionType action) {
    switch (action) {
        case ActionType::UserTemp:              return UserTempPaths();
        case ActionType::WindowsTemp:           return WindowsTempPaths();
        case ActionType::WerReports:            return WerPaths();
        case ActionType::RecycleBin:            return RecycleBinPaths();
        case ActionType::DeliveryOptimization:  return DeliveryOptimizationPaths();
        case ActionType::WindowsUpdateCache:    return WindowsUpdatePaths();
        case ActionType::CrashDumps:             return CrashDumpPaths();
        case ActionType::ShaderCaches:           return ShaderCachePaths();
        case ActionType::BrowserCaches:          return BrowserCachePaths();
        case ActionType::CommunicationCaches:    return CommunicationCachePaths();
        case ActionType::ThumbnailCaches:        return ThumbnailCachePaths();
        case ActionType::DisableHibernation:     return HibernationPaths();
        case ActionType::DismCleanup:            return {};
    }
    return {};
}

bool ProtectRecentForAction(ActionType action) {
    switch (action) {
        case ActionType::UserTemp:
        case ActionType::WindowsTemp:
        case ActionType::WerReports:
        case ActionType::CrashDumps:
        case ActionType::ShaderCaches:
        case ActionType::BrowserCaches:
        case ActionType::CommunicationCaches:
        case ActionType::ThumbnailCaches:
            return true;
        default:
            return false;
    }
}

std::wstring SystemDirectory() {
    std::array<wchar_t, MAX_PATH> buffer{};
    const UINT length = GetSystemDirectoryW(buffer.data(), static_cast<UINT>(buffer.size()));
    return length > 0 && length < buffer.size() ? std::wstring(buffer.data(), length) : WindowsDir() + L"\\System32";
}

bool RunSystemTool(const wchar_t* executable, const std::wstring& arguments, DWORD* exitCode = nullptr) {
    const std::wstring application = (fs::path(SystemDirectory()) / executable).wstring();
    std::wstring commandLine = L"\"" + application + L"\" " + arguments;
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESHOWWINDOW;
    startup.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION process{};
    std::vector<wchar_t> command(commandLine.begin(), commandLine.end());
    command.push_back(L'\0');

    BOOL ok = CreateProcessW(
        application.c_str(),
        command.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &startup,
        &process
    );
    if (!ok) return false;

    bool cancellationNoted = false;
    for (;;) {
        const DWORD wait = WaitForSingleObject(process.hProcess, 250);
        if (wait == WAIT_OBJECT_0) break;
        if (wait == WAIT_FAILED) {
            CloseHandle(process.hThread);
            CloseHandle(process.hProcess);
            return false;
        }
        if (g_cancel.load() && !cancellationNoted) {
            cancellationNoted = true;
            PostStatus(loc::system::Waiting);
            PostLog(loc::system::NotForceStopped, LogSeverity::Warning);
        }
    }
    DWORD code = 1;
    GetExitCodeProcess(process.hProcess, &code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);

    if (exitCode) *exitCode = code;
    return code == 0;
}

bool QueryServiceState(SC_HANDLE service, DWORD& state) {
    SERVICE_STATUS_PROCESS status{};
    DWORD needed = 0;
    if (!QueryServiceStatusEx(
            service,
            SC_STATUS_PROCESS_INFO,
            reinterpret_cast<LPBYTE>(&status),
            sizeof(status),
            &needed)) {
        return false;
    }
    state = status.dwCurrentState;
    return true;
}

bool WaitForServiceState(SC_HANDLE service, DWORD wantedState, DWORD timeoutMs) {
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);
    SERVICE_STATUS_PROCESS status{};
    DWORD needed = 0;

    while (std::chrono::steady_clock::now() < deadline) {
        if (!QueryServiceStatusEx(
                service,
                SC_STATUS_PROCESS_INFO,
                reinterpret_cast<LPBYTE>(&status),
                sizeof(status),
                &needed)) {
            return false;
        }
        if (status.dwCurrentState == wantedState) return true;
        Sleep(250);
    }
    return false;
}

struct ServiceRestore {
    std::wstring name;
    bool wasRunning{};
    bool readyForCleanup{};
    DWORD errorCode{};
    std::wstring failure;
};

ServiceRestore StopServiceForCleanup(const wchar_t* serviceName) {
    ServiceRestore restore{serviceName, false, false};

    SC_HANDLE manager = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!manager) {
        restore.errorCode = GetLastError();
        restore.failure = loc::system::ServiceManagerOpenFailed;
        return restore;
    }

    SC_HANDLE service = OpenServiceW(
        manager,
        serviceName,
        SERVICE_QUERY_STATUS | SERVICE_STOP | SERVICE_START
    );
    if (!service) {
        const DWORD openError = GetLastError();
        if (openError == ERROR_SERVICE_DOES_NOT_EXIST) restore.readyForCleanup = true;
        else {
            restore.errorCode = openError;
            restore.failure = loc::system::ServiceOpenFailed;
        }
        CloseServiceHandle(manager);
        return restore;
    }

    DWORD state = 0;
    if (!QueryServiceState(service, state)) {
        restore.errorCode = GetLastError();
        restore.failure = loc::system::ServiceStateFailed;
    } else if (state == SERVICE_STOPPED) {
        restore.readyForCleanup = true;
    } else {
        restore.wasRunning = state == SERVICE_RUNNING || state == SERVICE_START_PENDING || state == SERVICE_PAUSED;
        if (state == SERVICE_STOP_PENDING) {
            restore.readyForCleanup = WaitForServiceState(service, SERVICE_STOPPED, 20000);
        } else {
            SERVICE_STATUS status{};
            if (ControlService(service, SERVICE_CONTROL_STOP, &status)) {
                restore.readyForCleanup = WaitForServiceState(service, SERVICE_STOPPED, 20000);
            } else {
                restore.errorCode = GetLastError();
            }
        }
        if (!restore.readyForCleanup) {
            if (restore.errorCode == 0) restore.errorCode = ERROR_SERVICE_REQUEST_TIMEOUT;
            restore.failure = loc::system::ServiceStopFailed;
        }
    }

    CloseServiceHandle(service);
    CloseServiceHandle(manager);
    return restore;
}

bool RestoreService(const ServiceRestore& restore, DWORD* errorCode = nullptr) {
    if (errorCode) *errorCode = ERROR_SUCCESS;
    if (!restore.wasRunning) return true;

    SC_HANDLE manager = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!manager) {
        if (errorCode) *errorCode = GetLastError();
        return false;
    }

    SC_HANDLE service = OpenServiceW(
        manager,
        restore.name.c_str(),
        SERVICE_QUERY_STATUS | SERVICE_START
    );
    bool restored = false;
    if (service) {
        DWORD state = 0;
        if (QueryServiceState(service, state) && state == SERVICE_RUNNING) {
            restored = true;
        } else if (StartServiceW(service, 0, nullptr) || GetLastError() == ERROR_SERVICE_ALREADY_RUNNING) {
            restored = WaitForServiceState(service, SERVICE_RUNNING, 20000);
        }
        if (!restored && errorCode) {
            const DWORD lastError = GetLastError();
            *errorCode = lastError == ERROR_SUCCESS ? ERROR_SERVICE_REQUEST_TIMEOUT : lastError;
        }
        CloseServiceHandle(service);
    } else if (errorCode) {
        *errorCode = GetLastError();
    }
    CloseServiceHandle(manager);
    return restored;
}

CleanupStats ExecuteAction(ActionType action, bool protectRecent, uint64_t expectedBytes,
                           uint64_t expectedFiles) {
    CleanupStats result;
    switch (action) {
        case ActionType::UserTemp:
        case ActionType::WindowsTemp:
        case ActionType::WerReports:
        case ActionType::CrashDumps:
        case ActionType::ShaderCaches:
        case ActionType::BrowserCaches:
        case ActionType::CommunicationCaches:
        case ActionType::ThumbnailCaches:
            return CleanupPaths(PathsForAction(action), protectRecent && ProtectRecentForAction(action));

        case ActionType::RecycleBin: {
            if (g_cancel.load()) return result;
            const std::wstring root = SystemDriveRoot();
            HRESULT hr = SHEmptyRecycleBinW(
                g_main,
                root.c_str(),
                SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND
            );
            if (SUCCEEDED(hr)) {
                result.deletedBytes = expectedBytes;
                result.deletedFiles = expectedFiles;
            }
            else RecordCleanupIssue(result, CleanupIssueKind::RecycleBinFailure, false,
                                    L"SHEmptyRecycleBinW", static_cast<int64_t>(hr));
            return result;
        }

        case ActionType::DeliveryOptimization: {
            auto dosvc = StopServiceForCleanup(L"DoSvc");
            if (dosvc.readyForCleanup && !g_cancel.load()) {
                const auto files = CleanupPaths(DeliveryOptimizationPaths(), false);
                MergeCleanupStatsSameCategory(result, files);
            } else if (g_cancel.load()) {
                RecordCleanupIssue(result, CleanupIssueKind::CancelledOperation, false, loc::system::DeliveryOptimization);
            } else {
                RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                   dosvc.name + L": " + dosvc.failure, dosvc.errorCode);
            }
            DWORD restoreCode = ERROR_SUCCESS;
            if (!RestoreService(dosvc, &restoreCode)) {
                RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                   dosvc.name + L": " + loc::system::ServiceRestoreFailed, restoreCode);
            }
            return result;
        }

        case ActionType::WindowsUpdateCache: {
            auto wu = StopServiceForCleanup(L"wuauserv");
            auto bits = StopServiceForCleanup(L"bits");
            if (wu.readyForCleanup && bits.readyForCleanup && !g_cancel.load()) {
                const auto files = CleanupPaths(WindowsUpdatePaths(), false);
                MergeCleanupStatsSameCategory(result, files);
            } else if (g_cancel.load()) {
                RecordCleanupIssue(result, CleanupIssueKind::CancelledOperation, false, loc::system::WindowsUpdate);
            } else {
                if (!wu.readyForCleanup) {
                    RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                       wu.name + L": " + wu.failure, wu.errorCode);
                }
                if (!bits.readyForCleanup) {
                    RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                       bits.name + L": " + bits.failure, bits.errorCode);
                }
            }
            DWORD bitsRestoreCode = ERROR_SUCCESS;
            if (!RestoreService(bits, &bitsRestoreCode)) {
                RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                   bits.name + L": " + loc::system::ServiceRestoreFailed, bitsRestoreCode);
            }
            DWORD wuRestoreCode = ERROR_SUCCESS;
            if (!RestoreService(wu, &wuRestoreCode)) {
                RecordCleanupIssue(result, CleanupIssueKind::ServiceFailure, false,
                                   wu.name + L": " + loc::system::ServiceRestoreFailed, wuRestoreCode);
            }
            return result;
        }

        case ActionType::DismCleanup: {
            PostLog(loc::system::DismWarning, LogSeverity::Warning);
            DWORD code = 1;
            if (!RunSystemTool(L"dism.exe", L"/Online /Cleanup-Image /StartComponentCleanup", &code) || code != 0) {
                RecordCleanupIssue(result, CleanupIssueKind::SystemToolFailure, false, L"DISM", code);
            }
            return result;
        }

        case ActionType::DisableHibernation: {
            DWORD code = 1;
            if (RunSystemTool(L"powercfg.exe", L"/hibernate off", &code) && code == 0) {
                result.deletedBytes = expectedBytes;
                result.deletedFiles = expectedBytes > 0 ? 1 : 0;
            } else {
                RecordCleanupIssue(result, CleanupIssueKind::SystemToolFailure, false, L"powercfg", code);
            }
            return result;
        }
    }
    RecordCleanupIssue(result, CleanupIssueKind::UnexpectedFailure, false, loc::system::UnknownCleanupAction);
    return result;
}
// ------------------------------ v2 interface ------------------------------

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void LayoutUi();
void ApplyUiState(AppPhase phase);
void UpdateSelectedUi();
void UpdateDriveUi();
void UpdateFoundUi();
void RestoreScanSummaryUi();
void ShowCleanupSummaryUi(const CleanupStats& stats, uint64_t freedBytes, bool freedBytesMeasured);
void ApplyLanguageUi();

int Dip(int value) {
    return MulDiv(value, static_cast<int>(g_dpi), 96);
}

COLORREF BlendColor(COLORREF from, COLORREF to, int percent) {
    const int inverse = 100 - percent;
    return RGB(
        (GetRValue(from) * inverse + GetRValue(to) * percent) / 100,
        (GetGValue(from) * inverse + GetGValue(to) * percent) / 100,
        (GetBValue(from) * inverse + GetBValue(to) * percent) / 100
    );
}

HFONT MakeFont(int logicalPixels, int weight, const wchar_t* face = L"Segoe UI Variable Text") {
    return CreateFontW(
        -Dip(logicalPixels), 0, 0, 0, weight,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        face
    );
}

void DeleteFonts() {
    for (HFONT font : {g_fontHero, g_fontTitle, g_fontSubtitle, g_fontValue, g_fontNormal,
                       g_fontSmall, g_fontBold, g_fontTiny, g_fontMono}) {
        if (font) DeleteObject(font);
    }
    g_fontHero = g_fontTitle = g_fontSubtitle = g_fontValue = g_fontNormal = nullptr;
    g_fontSmall = g_fontBold = g_fontTiny = g_fontMono = nullptr;
}

void CreateFonts() {
    DeleteFonts();
    g_fontHero = MakeFont(29, FW_SEMIBOLD);
    g_fontTitle = MakeFont(18, FW_SEMIBOLD);
    g_fontSubtitle = MakeFont(14, FW_SEMIBOLD);
    g_fontValue = MakeFont(23, FW_BOLD);
    g_fontNormal = MakeFont(14, FW_NORMAL);
    g_fontSmall = MakeFont(12, FW_NORMAL);
    g_fontBold = MakeFont(13, FW_SEMIBOLD);
    g_fontTiny = MakeFont(10, FW_SEMIBOLD);
    g_fontMono = MakeFont(12, FW_NORMAL, L"Cascadia Mono");
}

void SetControlFont(HWND control, HFONT font) {
    if (control) SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
}

void ApplyFonts() {
    SetControlFont(g_title, g_fontHero);
    SetControlFont(g_subtitle, g_fontNormal);
    for (HWND control : {g_freeCaption, g_foundCaption, g_selectedCaption, g_itemsHint, g_logHint,
                         g_freeDetail, g_foundDetail, g_selectedDetail, g_statusDetail}) {
        SetControlFont(control, g_fontSmall);
    }
    for (HWND control : {g_freeValue, g_foundValue, g_selectedValue}) SetControlFont(control, g_fontValue);
    for (HWND control : {g_itemsTitle, g_logTitle, g_status}) SetControlFont(control, g_fontSubtitle);
    SetControlFont(g_log, g_fontMono);
    SetControlFont(g_protectRecentCheck, g_fontSmall);
    for (HWND control : {g_scanButton, g_cleanButton, g_stopButton}) SetControlFont(control, g_fontBold);
    for (HWND control : {g_recommendedButton, g_safeButton, g_clearSelectionButton, g_exportButton,
                         g_languageButton, g_storageButton, g_clearLogButton}) {
        SetControlFont(control, g_fontSmall);
    }
    for (auto& item : g_items) SetControlFont(item.checkbox, g_fontBold);
}

HWND MakeStatic(HWND parent, const wchar_t* text, HFONT font,
                DWORD style = SS_LEFT, bool transparent = true) {
    HWND control = CreateWindowExW(
        transparent ? WS_EX_TRANSPARENT : 0, L"STATIC", text,
        WS_CHILD | WS_VISIBLE | style,
        0, 0, 0, 0,
        parent, nullptr, g_instance, nullptr
    );
    SetControlFont(control, font);
    return control;
}

void DrawRoundedPanel(HDC dc, const RECT& rect, COLORREF fill, COLORREF border = COLOR_BORDER, int radius = 16) {
    HBRUSH brush = CreateSolidBrush(fill);
    HPEN pen = CreatePen(PS_SOLID, 1, border);
    const auto oldBrush = SelectObject(dc, brush);
    const auto oldPen = SelectObject(dc, pen);
    RoundRect(dc, rect.left, rect.top, rect.right, rect.bottom, Dip(radius), Dip(radius));
    SelectObject(dc, oldBrush);
    SelectObject(dc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

void DrawTextStyled(HDC dc, const std::wstring& text, RECT rect, HFONT font, COLORREF color, UINT format) {
    const auto oldFont = SelectObject(dc, font);
    const int oldMode = SetBkMode(dc, TRANSPARENT);
    const COLORREF oldColor = SetTextColor(dc, color);
    DrawTextW(dc, text.c_str(), -1, &rect, format);
    SetTextColor(dc, oldColor);
    SetBkMode(dc, oldMode);
    SelectObject(dc, oldFont);
}

void DrawTextStyled(HDC dc, const localization::LocalizedString& text, RECT rect, HFONT font, COLORREF color, UINT format) {
    DrawTextStyled(dc, text.str(), rect, font, color, format);
}

std::wstring RiskLabel(RiskLevel risk) {
    switch (risk) {
        case RiskLevel::Safe: return loc::risk::Safe.str();
        case RiskLevel::Caution: return loc::risk::Caution.str();
        case RiskLevel::System: return loc::risk::System.str();
    }
    return L"";
}

COLORREF RiskColor(RiskLevel risk) {
    switch (risk) {
        case RiskLevel::Safe: return COLOR_GREEN;
        case RiskLevel::Caution: return COLOR_YELLOW;
        case RiskLevel::System: return COLOR_RED;
    }
    return COLOR_MUTED;
}

std::wstring ItemSizeText(const CleanerItem& item) {
    if (!item.scanned) return loc::item::NotScanned.str();
    if (item.action == ActionType::DismCleanup) return loc::item::DismEstimate.str();
    return FormatBytes(item.scan.bytes);
}

void DrawCleanerCard(HWND hwnd, HDC outputDc, size_t index) {
    if (index >= g_items.size()) return;
    auto& item = g_items[index];

    RECT client{};
    GetClientRect(hwnd, &client);
    const int width = client.right;
    const int height = client.bottom;
    if (width <= 0 || height <= 0) return;

    HDC dc = CreateCompatibleDC(outputDc);
    HBITMAP bitmap = CreateCompatibleBitmap(outputDc, width, height);
    const auto oldBitmap = SelectObject(dc, bitmap);

    const bool checked = SendMessageW(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
    const bool enabled = IsWindowEnabled(hwnd) != FALSE;
    const bool focused = GetFocus() == hwnd;
    const bool active = static_cast<int>(index) == g_activeItem;

    COLORREF fill = item.hovered ? COLOR_CARD_HOVER : COLOR_CARD;
    COLORREF border = checked ? BlendColor(COLOR_ACCENT, COLOR_BORDER, 32) : COLOR_BORDER;
    if (active) {
        fill = BlendColor(COLOR_CARD, COLOR_ACCENT, 16);
        border = COLOR_ACCENT;
    }
    if (!enabled) fill = BlendColor(fill, COLOR_BG, 30);

    RECT card{0, 0, width, height};
    DrawRoundedPanel(dc, card, fill, border, 13);

    RECT accent{0, 0, Dip(4), height};
    if (checked || active) {
        HBRUSH accentBrush = CreateSolidBrush(active ? COLOR_ACCENT_2 : COLOR_ACCENT);
        FillRect(dc, &accent, accentBrush);
        DeleteObject(accentBrush);
    }

    const int checkSize = Dip(20);
    RECT check{Dip(15), (height - checkSize) / 2, Dip(15) + checkSize, (height + checkSize) / 2};
    DrawRoundedPanel(dc, check, checked ? COLOR_ACCENT : COLOR_PANEL_2,
                     checked ? COLOR_ACCENT : COLOR_SUBTLE, 6);
    if (checked) {
        HPEN pen = CreatePen(PS_SOLID, Dip(2), RGB(255, 255, 255));
        const auto oldPen = SelectObject(dc, pen);
        MoveToEx(dc, check.left + Dip(5), check.top + Dip(10), nullptr);
        LineTo(dc, check.left + Dip(9), check.top + Dip(14));
        LineTo(dc, check.left + Dip(16), check.top + Dip(6));
        SelectObject(dc, oldPen);
        DeleteObject(pen);
    }

    const int textLeft = Dip(48);
    const int badgeWidth = Dip(116);
    RECT titleRect{textLeft, Dip(7), width - badgeWidth - Dip(12), Dip(29)};
    DrawTextStyled(dc, item.title, titleRect, g_fontBold,
                   enabled ? COLOR_TEXT : COLOR_SUBTLE,
                   DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    RECT sizeRect{width - badgeWidth, Dip(7), width - Dip(12), Dip(29)};
    DrawTextStyled(dc, ItemSizeText(item), sizeRect, g_fontTiny,
                   item.scanned ? COLOR_ACCENT : COLOR_SUBTLE,
                   DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    std::wstring description = item.risk == RiskLevel::Safe
        ? item.description
        : RiskLabel(item.risk) + L" · " + item.description;
    if (item.scanned && item.scan.errors > 0) description += L" · " + std::wstring(loc::item::SomeUnavailable);
    RECT descriptionRect{textLeft, Dip(29), width - Dip(12), height - Dip(5)};
    DrawTextStyled(dc, description, descriptionRect, g_fontSmall,
                   item.risk == RiskLevel::Safe ? COLOR_MUTED : RiskColor(item.risk),
                   DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

    if (focused) {
        HPEN focusPen = CreatePen(PS_SOLID, Dip(2), COLOR_ACCENT);
        const auto oldPen = SelectObject(dc, focusPen);
        const auto oldBrush = SelectObject(dc, GetStockObject(NULL_BRUSH));
        RoundRect(dc, Dip(2), Dip(2), width - Dip(2), height - Dip(2), Dip(12), Dip(12));
        SelectObject(dc, oldBrush);
        SelectObject(dc, oldPen);
        DeleteObject(focusPen);
    }

    BitBlt(outputDc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
    SelectObject(dc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(dc);
}

LRESULT CALLBACK CleanerItemSubclass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
                                     UINT_PTR, DWORD_PTR reference) {
    const size_t index = static_cast<size_t>(reference);
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT paint{};
            HDC dc = BeginPaint(hwnd, &paint);
            DrawCleanerCard(hwnd, dc, index);
            EndPaint(hwnd, &paint);
            return 0;
        }
        case WM_PRINTCLIENT:
            DrawCleanerCard(hwnd, reinterpret_cast<HDC>(wParam), index);
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_MOUSEMOVE:
            if (index < g_items.size() && !g_items[index].hovered) {
                g_items[index].hovered = true;
                TRACKMOUSEEVENT tracking{sizeof(tracking), TME_LEAVE, hwnd, 0};
                TrackMouseEvent(&tracking);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            break;
        case WM_MOUSELEAVE:
            if (index < g_items.size()) g_items[index].hovered = false;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_ENABLE:
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        case BM_SETCHECK: {
            const LRESULT result = DefSubclassProc(hwnd, message, wParam, lParam);
            InvalidateRect(hwnd, nullptr, FALSE);
            return result;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(hwnd, CleanerItemSubclass, 1);
            break;
    }
    return DefSubclassProc(hwnd, message, wParam, lParam);
}

void DrawProtectRecentCheck(HWND hwnd, HDC outputDc) {
    RECT client{};
    GetClientRect(hwnd, &client);
    const int width = client.right;
    const int height = client.bottom;
    if (width <= 0 || height <= 0) return;

    HDC dc = CreateCompatibleDC(outputDc);
    HBITMAP bitmap = CreateCompatibleBitmap(outputDc, width, height);
    const auto oldBitmap = SelectObject(dc, bitmap);
    HBRUSH background = CreateSolidBrush(COLOR_PANEL);
    FillRect(dc, &client, background);
    DeleteObject(background);

    const bool checked = SendMessageW(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
    const bool enabled = IsWindowEnabled(hwnd) != FALSE;
    const bool focused = GetFocus() == hwnd;
    const int checkSize = Dip(18);
    RECT check{0, (height - checkSize) / 2, checkSize, (height + checkSize) / 2};
    DrawRoundedPanel(dc, check, checked ? COLOR_GREEN : COLOR_PANEL_2,
                     checked ? COLOR_GREEN : COLOR_SUBTLE, 5);
    if (checked) {
        HPEN pen = CreatePen(PS_SOLID, Dip(2), COLOR_BG);
        const auto oldPen = SelectObject(dc, pen);
        MoveToEx(dc, check.left + Dip(4), check.top + Dip(9), nullptr);
        LineTo(dc, check.left + Dip(8), check.top + Dip(13));
        LineTo(dc, check.left + Dip(15), check.top + Dip(5));
        SelectObject(dc, oldPen);
        DeleteObject(pen);
    }

    wchar_t text[256]{};
    GetWindowTextW(hwnd, text, static_cast<int>(std::size(text)));
    RECT textRect{Dip(27), 0, width, height};
    DrawTextStyled(dc, text, textRect, g_fontSmall, enabled ? COLOR_MUTED : COLOR_SUBTLE,
                   DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
    if (focused) {
        RECT focus = client;
        focus.left = Dip(24);
        InflateRect(&focus, -Dip(1), -Dip(1));
        DrawFocusRect(dc, &focus);
    }

    BitBlt(outputDc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
    SelectObject(dc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(dc);
}

LRESULT CALLBACK ProtectRecentSubclass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
                                       UINT_PTR, DWORD_PTR) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT paint{};
            HDC dc = BeginPaint(hwnd, &paint);
            DrawProtectRecentCheck(hwnd, dc);
            EndPaint(hwnd, &paint);
            return 0;
        }
        case WM_PRINTCLIENT:
            DrawProtectRecentCheck(hwnd, reinterpret_cast<HDC>(wParam));
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_ENABLE:
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        case BM_SETCHECK: {
            const LRESULT result = DefSubclassProc(hwnd, message, wParam, lParam);
            InvalidateRect(hwnd, nullptr, FALSE);
            return result;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(hwnd, ProtectRecentSubclass, 1);
            break;
    }
    return DefSubclassProc(hwnd, message, wParam, lParam);
}

void DrawProgressControl(HWND hwnd, HDC outputDc) {
    RECT client{};
    GetClientRect(hwnd, &client);
    const int width = client.right;
    const int height = client.bottom;
    if (width <= 0 || height <= 0) return;

    HDC dc = CreateCompatibleDC(outputDc);
    HBITMAP bitmap = CreateCompatibleBitmap(outputDc, width, height);
    const auto oldBitmap = SelectObject(dc, bitmap);
    DrawRoundedPanel(dc, client, COLOR_PANEL_2, COLOR_PANEL_2, 6);
    const int position = static_cast<int>(SendMessageW(hwnd, PBM_GETPOS, 0, 0));
    if (position > 0) {
        RECT filled = client;
        filled.right = (std::max)(Dip(4), width * (std::min)(100, position) / 100);
        DrawRoundedPanel(dc, filled, COLOR_ACCENT, COLOR_ACCENT, 6);
    }
    BitBlt(outputDc, 0, 0, width, height, dc, 0, 0, SRCCOPY);
    SelectObject(dc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(dc);
}

LRESULT CALLBACK ProgressSubclass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
                                  UINT_PTR, DWORD_PTR) {
    switch (message) {
        case WM_PAINT: {
            PAINTSTRUCT paint{};
            HDC dc = BeginPaint(hwnd, &paint);
            DrawProgressControl(hwnd, dc);
            EndPaint(hwnd, &paint);
            return 0;
        }
        case WM_PRINTCLIENT:
            DrawProgressControl(hwnd, reinterpret_cast<HDC>(wParam));
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case PBM_SETPOS:
        case PBM_DELTAPOS:
        case PBM_SETSTEP: {
            const LRESULT result = DefSubclassProc(hwnd, message, wParam, lParam);
            InvalidateRect(hwnd, nullptr, FALSE);
            return result;
        }
        case WM_NCDESTROY:
            RemoveWindowSubclass(hwnd, ProgressSubclass, 1);
            break;
    }
    return DefSubclassProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK OwnerButtonSubclass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam,
                                     UINT_PTR, DWORD_PTR) {
    switch (message) {
        case WM_MOUSEMOVE:
            if (GetWindowLongPtrW(hwnd, GWLP_USERDATA) == 0) {
                SetWindowLongPtrW(hwnd, GWLP_USERDATA, 1);
                TRACKMOUSEEVENT tracking{sizeof(tracking), TME_LEAVE, hwnd, 0};
                TrackMouseEvent(&tracking);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
            break;
        case WM_MOUSELEAVE:
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, 0);
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        case WM_ENABLE:
            InvalidateRect(hwnd, nullptr, FALSE);
            break;
        case WM_NCDESTROY:
            RemoveWindowSubclass(hwnd, OwnerButtonSubclass, 1);
            break;
    }
    return DefSubclassProc(hwnd, message, wParam, lParam);
}

HWND MakeButton(HWND parent, const wchar_t* text, int id, HFONT font) {
    HWND control = CreateWindowExW(
        0, L"BUTTON", text,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_OWNERDRAW,
        0, 0, 0, 0,
        parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), g_instance, nullptr
    );
    SetControlFont(control, font);
    SetWindowSubclass(control, OwnerButtonSubclass, 1, 0);
    return control;
}

LRESULT DrawOwnerButton(const DRAWITEMSTRUCT* item) {
    if (!item || item->CtlType != ODT_BUTTON) return FALSE;

    const bool disabled = (item->itemState & ODS_DISABLED) != 0;
    const bool pressed = (item->itemState & ODS_SELECTED) != 0;
    const bool focused = (item->itemState & ODS_FOCUS) != 0;
    const bool hovered = GetWindowLongPtrW(item->hwndItem, GWLP_USERDATA) != 0;

    COLORREF fill = COLOR_PANEL_2;
    COLORREF border = COLOR_BORDER;
    COLORREF textColor = COLOR_TEXT;
    if (item->CtlID == ID_CLEAN) {
        fill = COLOR_ACCENT_2;
        border = COLOR_ACCENT_2;
    } else if (item->CtlID == ID_SCAN) {
        fill = COLOR_ACCENT;
        border = COLOR_ACCENT;
    } else if (item->CtlID == ID_STOP) {
        fill = RGB(124, 45, 64);
        border = COLOR_RED;
    } else if (item->CtlID == ID_PRESET_RECOMMENDED) {
        border = COLOR_ACCENT;
    } else if (item->CtlID == ID_STORAGE_SETTINGS || item->CtlID == ID_LANGUAGE) {
        fill = COLOR_CARD;
    }

    if (hovered && !disabled) fill = BlendColor(fill, RGB(255, 255, 255), 10);
    if (pressed && !disabled) fill = BlendColor(fill, COLOR_BG, 22);
    if (disabled) {
        fill = BlendColor(fill, COLOR_BG, 42);
        border = BlendColor(border, COLOR_BG, 35);
        textColor = COLOR_SUBTLE;
    }

    RECT rect = item->rcItem;
    DrawRoundedPanel(item->hDC, rect, fill, border, 12);

    wchar_t text[256]{};
    GetWindowTextW(item->hwndItem, text, static_cast<int>(std::size(text)));
    HFONT font = (item->CtlID == ID_SCAN || item->CtlID == ID_CLEAN || item->CtlID == ID_STOP)
        ? g_fontBold : g_fontSmall;
    DrawTextStyled(item->hDC, text, rect, font, textColor,
                   DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_HIDEPREFIX | DT_END_ELLIPSIS);

    if (focused) {
        RECT focus = rect;
        InflateRect(&focus, -Dip(3), -Dip(3));
        HPEN pen = CreatePen(PS_SOLID, Dip(2), RGB(255, 255, 255));
        const auto oldPen = SelectObject(item->hDC, pen);
        const auto oldBrush = SelectObject(item->hDC, GetStockObject(NULL_BRUSH));
        RoundRect(item->hDC, focus.left, focus.top, focus.right, focus.bottom, Dip(10), Dip(10));
        SelectObject(item->hDC, oldBrush);
        SelectObject(item->hDC, oldPen);
        DeleteObject(pen);
    }
    return TRUE;
}

void AppendRichSegment(const std::wstring& text, COLORREF color) {
    if (!g_log) return;
    SendMessageW(g_log, EM_SETSEL, static_cast<WPARAM>(-1), static_cast<LPARAM>(-1));
    CHARFORMAT2W format{};
    format.cbSize = sizeof(format);
    format.dwMask = CFM_COLOR;
    format.crTextColor = color;
    SendMessageW(g_log, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>(&format));
    SendMessageW(g_log, EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(text.c_str()));
}

void AppendLogUi(const std::wstring& text, LogSeverity severity = LogSeverity::Info) {
    SYSTEMTIME time{};
    GetLocalTime(&time);
    std::wostringstream timestamp;
    timestamp << L"[" << std::setfill(L'0') << std::setw(2) << time.wHour
              << L":" << std::setw(2) << time.wMinute
              << L":" << std::setw(2) << time.wSecond << L"] ";

    COLORREF color = COLOR_TEXT;
    if (severity == LogSeverity::Error) color = COLOR_RED;
    else if (severity == LogSeverity::Warning) color = COLOR_YELLOW;
    else if (severity == LogSeverity::Success) color = COLOR_GREEN;

    AppendRichSegment(timestamp.str(), COLOR_SUBTLE);
    AppendRichSegment(text + L"\r\n", color);
    SendMessageW(g_log, EM_SCROLLCARET, 0, 0);
}

void AppendLogUi(const localization::LocalizedString& text, LogSeverity severity = LogSeverity::Info) {
    AppendLogUi(text.str(), severity);
}

void SetStatusText(const std::wstring& title, const std::wstring& detail) {
    SetWindowTextW(g_status, title.c_str());
    SetWindowTextW(g_statusDetail, detail.c_str());
    RedrawWindow(g_status, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    RedrawWindow(g_statusDetail, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, g_status, OBJID_CLIENT, CHILDID_SELF);
    InvalidateRect(g_main, &g_footerRect, FALSE);
}

void SetStatusText(const localization::LocalizedString& title, const localization::LocalizedString& detail) {
    SetStatusText(title.str(), detail.str());
}

void SetStatusText(const localization::LocalizedString& title, const std::wstring& detail) {
    SetStatusText(title.str(), detail);
}

void SetStatusText(const std::wstring& title, const localization::LocalizedString& detail) {
    SetStatusText(title, detail.str());
}

const localization::CategoryText& LocalizedCategoryForAction(ActionType action) {
    switch (action) {
        case ActionType::UserTemp:              return loc::category::UserTemp;
        case ActionType::WindowsTemp:           return loc::category::WindowsTemp;
        case ActionType::WerReports:            return loc::category::ErrorReports;
        case ActionType::DeliveryOptimization:  return loc::category::DeliveryOptimization;
        case ActionType::ThumbnailCaches:       return loc::category::Thumbnails;
        case ActionType::BrowserCaches:         return loc::category::Browsers;
        case ActionType::CommunicationCaches:   return loc::category::Messaging;
        case ActionType::ShaderCaches:          return loc::category::Shaders;
        case ActionType::CrashDumps:            return loc::category::CrashDumps;
        case ActionType::RecycleBin:            return loc::category::RecycleBin;
        case ActionType::WindowsUpdateCache:    return loc::category::UpdateDownloads;
        case ActionType::DismCleanup:            return loc::category::ComponentStore;
        case ActionType::DisableHibernation:     return loc::category::DisableHibernation;
    }
    return loc::category::UserTemp;
}

std::wstring LanguageButtonText() {
    return std::wstring(localization::LanguageName(localization::GetLanguage())) + L"  ▾";
}

void RefreshStatusLanguage() {
    switch (g_phase) {
        case AppPhase::Scanning:
            SetStatusText(loc::scan::Status, loc::scan::Detail);
            return;
        case AppPhase::ScanComplete:
            SetStatusText(loc::scan::Complete, loc::scan::CompleteDetail);
            return;
        case AppPhase::Cleaning:
            SetStatusText(loc::cleanup::InProgress, loc::cleanup::InProgressDetail);
            return;
        case AppPhase::Cancelling:
            SetStatusText(loc::cleanup::Stopping, loc::cleanup::StoppingDetail);
            return;
        case AppPhase::Completed:
        case AppPhase::CompletedWithSkips:
        case AppPhase::PartialFailure:
            if (g_showCleanupSummary && g_hasLastCleanup) {
                const std::wstring details = loc::CleanupMetrics(
                    g_lastCleanupStats.deletedFiles,
                    FormatMeasuredBytes(g_lastFreedBytes, g_lastFreedBytesMeasured),
                    g_lastCleanupStats.skippedFiles,
                    g_lastCleanupStats.errors);
                if (g_lastCleanupStats.errors > 0)
                    SetStatusText(loc::cleanup::CompletedErrors, details);
                else if (g_lastCleanupStats.skippedFiles > 0)
                    SetStatusText(loc::cleanup::CompletedSkips, details);
                else
                    SetStatusText(loc::cleanup::CompletedSuccess, details);
            } else if (g_phase == AppPhase::PartialFailure) {
                SetStatusText(loc::scan::Failed, loc::scan::FailedDetail);
            } else {
                SetStatusText(loc::ui::ReadyToScan, loc::ui::RecommendedSelected);
            }
            return;
        case AppPhase::ReadyUnscanned:
        default:
            SetStatusText(loc::ui::ReadyToScan, loc::ui::RecommendedSelected);
            return;
    }
}

void ApplyLanguageUi() {
    if (!g_main) return;

    SetWindowTextW(g_main, loc::app::WindowTitle);
    SetWindowTextW(g_title, loc::app::Name);
    SetWindowTextW(g_subtitle, loc::ui::Subtitle);
    SetWindowTextW(g_storageButton, loc::ui::WindowsStorage);

    if (g_languageButton) {
        const std::wstring languageText = LanguageButtonText();
        SetWindowTextW(g_languageButton, languageText.c_str());
    }

    SetWindowTextW(g_itemsTitle, loc::ui::ItemsToClean);
    SetWindowTextW(g_itemsHint, loc::ui::ItemsHint);
    SetWindowTextW(g_recommendedButton, loc::ui::Recommended);
    SetWindowTextW(g_safeButton, loc::ui::AllSafe);
    SetWindowTextW(g_clearSelectionButton, loc::ui::Reset);
    SetWindowTextW(g_protectRecentCheck, loc::ui::ProtectRecent);

    SetWindowTextW(g_logTitle, loc::ui::Activity);
    SetWindowTextW(g_logHint, loc::ui::ActivityHint);
    SetWindowTextW(g_exportButton, loc::ui::ExportTxt);
    SetWindowTextW(g_clearLogButton, loc::ui::Clear);

    for (auto& item : g_items) {
        const auto& text = LocalizedCategoryForAction(item.action);
        item.title = text.title.c_str();
        item.description = text.description.c_str();
        if (item.checkbox) {
            SetWindowTextW(item.checkbox, item.title.c_str());
            InvalidateRect(item.checkbox, nullptr, FALSE);
        }
    }

    UpdateDriveUi();
    if (g_showCleanupSummary && g_hasLastCleanup) {
        ShowCleanupSummaryUi(g_lastCleanupStats, g_lastFreedBytes, g_lastFreedBytesMeasured);
    } else {
        RestoreScanSummaryUi();
        UpdateFoundUi();
        UpdateSelectedUi();
    }

    SetWindowTextW(g_stopButton, g_phase == AppPhase::Cancelling ? loc::ui::Stopping : loc::ui::Stop);
    SetWindowTextW(g_scanButton, g_hasScanned ? loc::ui::ScanAgain : loc::ui::Scan);
    SetWindowTextW(g_cleanButton, loc::ui::CleanUp);
    UpdateSelectedUi();
    RefreshStatusLanguage();
    LayoutUi();
    InvalidateRect(g_main, nullptr, TRUE);
}

void ShowLanguageMenu(HWND owner) {
    if (!g_languageButton || g_busy.load()) return;

    HMENU menu = CreatePopupMenu();
    if (!menu) return;

    const auto current = localization::GetLanguage();
    AppendMenuW(menu, MF_STRING | (current == localization::Language::English ? MF_CHECKED : 0),
                ID_LANGUAGE_ENGLISH, localization::LanguageName(localization::Language::English));
    AppendMenuW(menu, MF_STRING | (current == localization::Language::Ukrainian ? MF_CHECKED : 0),
                ID_LANGUAGE_UKRAINIAN, localization::LanguageName(localization::Language::Ukrainian));
    AppendMenuW(menu, MF_STRING | (current == localization::Language::Russian ? MF_CHECKED : 0),
                ID_LANGUAGE_RUSSIAN, localization::LanguageName(localization::Language::Russian));

    RECT buttonRect{};
    GetWindowRect(g_languageButton, &buttonRect);
    const int command = TrackPopupMenu(
        menu, TPM_RETURNCMD | TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_NONOTIFY,
        buttonRect.right, buttonRect.bottom, 0, owner, nullptr);
    DestroyMenu(menu);

    localization::Language selected = current;
    if (command == ID_LANGUAGE_ENGLISH) selected = localization::Language::English;
    else if (command == ID_LANGUAGE_UKRAINIAN) selected = localization::Language::Ukrainian;
    else if (command == ID_LANGUAGE_RUSSIAN) selected = localization::Language::Russian;
    else return;

    if (selected != current) {
        localization::SetLanguage(selected);
        ApplyLanguageUi();
    }
}

void BuildItems(HWND parent) {
    g_items = {
        {ActionType::UserTemp,             loc::category::UserTemp.title.str(),             loc::category::UserTemp.description.str(),             true,  RiskLevel::Safe},
        {ActionType::WindowsTemp,          loc::category::WindowsTemp.title.str(),          loc::category::WindowsTemp.description.str(),          true,  RiskLevel::Safe},
        {ActionType::WerReports,           loc::category::ErrorReports.title.str(),          loc::category::ErrorReports.description.str(),          true,  RiskLevel::Safe},
        {ActionType::DeliveryOptimization, loc::category::DeliveryOptimization.title.str(), loc::category::DeliveryOptimization.description.str(), true,  RiskLevel::Safe},
        {ActionType::ThumbnailCaches,      loc::category::Thumbnails.title.str(),           loc::category::Thumbnails.description.str(),           true,  RiskLevel::Safe},
        {ActionType::BrowserCaches,        loc::category::Browsers.title.str(),             loc::category::Browsers.description.str(),             false, RiskLevel::Caution},
        {ActionType::CommunicationCaches,  loc::category::Messaging.title.str(),            loc::category::Messaging.description.str(),            false, RiskLevel::Caution},
        {ActionType::ShaderCaches,         loc::category::Shaders.title.str(),              loc::category::Shaders.description.str(),              false, RiskLevel::Caution},
        {ActionType::CrashDumps,           loc::category::CrashDumps.title.str(),           loc::category::CrashDumps.description.str(),           false, RiskLevel::Caution},
        {ActionType::RecycleBin,           loc::category::RecycleBin.title.str(),           loc::category::RecycleBin.description.str(),           false, RiskLevel::Caution},
        {ActionType::WindowsUpdateCache,   loc::category::UpdateDownloads.title.str(),      loc::category::UpdateDownloads.description.str(),      false, RiskLevel::Caution},
        {ActionType::DismCleanup,          loc::category::ComponentStore.title.str(),       loc::category::ComponentStore.description.str(),       false, RiskLevel::System},
        {ActionType::DisableHibernation,   loc::category::DisableHibernation.title.str(),   loc::category::DisableHibernation.description.str(),   false, RiskLevel::System}
    };

    for (size_t index = 0; index < g_items.size(); ++index) {
        auto& item = g_items[index];
        item.checkbox = CreateWindowExW(
            0, L"BUTTON", item.title.c_str(),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX | BS_FLAT,
            0, 0, 0, 0,
            parent,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(2000 + index)),
            g_instance,
            nullptr
        );
        SetWindowTheme(item.checkbox, L"", L"");
        SetWindowSubclass(item.checkbox, CleanerItemSubclass, 1, static_cast<DWORD_PTR>(index));
        SendMessageW(item.checkbox, BM_SETCHECK, item.defaultChecked ? BST_CHECKED : BST_UNCHECKED, 0);
    }
}

void ResetScanResults() {
    for (auto& item : g_items) {
        item.scan = {};
        item.scanned = false;
        InvalidateRect(item.checkbox, nullptr, FALSE);
    }
    g_hasScanned = false;
    g_activeItem = -1;
}

void UpdateDriveUi() {
    const DriveStats drive = QueryDriveStats();
    const std::wstring caption = std::wstring(loc::ui::SystemDrive) + L" " + SystemDriveLetter() + L":";
    SetWindowTextW(g_freeCaption, caption.c_str());
    if (!drive.valid) {
        SetWindowTextW(g_freeValue, loc::format::NotAvailable);
        SetWindowTextW(g_freeDetail, loc::ui::UnableDriveStatus);
    } else {
        SetWindowTextW(g_freeValue, FormatBytes(drive.free).c_str());
        const uint64_t used = drive.total > drive.free ? drive.total - drive.free : 0;
        const int percent = drive.total ? static_cast<int>((used * 100) / drive.total) : 0;
        const std::wstring detail = loc::DriveUsage(percent, FormatBytes(drive.total));
        SetWindowTextW(g_freeDetail, detail.c_str());
    }
    InvalidateRect(g_main, &g_freeCard, FALSE);
}

void RestoreScanSummaryUi() {
    SetWindowTextW(g_foundCaption, loc::ui::ScanResults);
    SetWindowTextW(g_selectedCaption, loc::ui::SelectedForCleanup);
}

void ShowCleanupSummaryUi(const CleanupStats& stats, uint64_t freedBytes, bool freedBytesMeasured) {
    SetWindowTextW(g_foundCaption, loc::ui::SpaceFreed);
    SetWindowTextW(g_foundValue, FormatMeasuredBytes(freedBytes, freedBytesMeasured).c_str());
    const std::wstring deleted = loc::report::FilesDeleted + std::to_wstring(stats.deletedFiles);
    SetWindowTextW(g_foundDetail, deleted.c_str());

    SetWindowTextW(g_selectedCaption, loc::ui::CleanupResult);
    const std::wstring result = stats.errors == 0
        ? loc::ui::NoErrors.str()
        : loc::Errors(stats.errors);
    SetWindowTextW(g_selectedValue, result.c_str());
    const std::wstring skipped = loc::report::FilesSkipped + std::to_wstring(stats.skippedFiles);
    SetWindowTextW(g_selectedDetail, skipped.c_str());

    SetWindowTextW(g_cleanButton, loc::ui::CleanUp);
    EnableWindow(g_cleanButton, FALSE);
    InvalidateRect(g_main, &g_foundCard, FALSE);
    InvalidateRect(g_main, &g_selectedCard, FALSE);
}

void UpdateFoundUi() {
    if (g_showCleanupSummary) return;
    uint64_t bytes = 0;
    uint64_t files = 0;
    int categories = 0;
    for (const auto& item : g_items) {
        if (!item.scanned) continue;
        AddBytesClamped(bytes, item.scan.bytes);
        files += item.scan.files;
        if (item.scan.bytes > 0 || item.action == ActionType::DismCleanup) ++categories;
    }

    SetWindowTextW(g_foundValue, FormatBytes(bytes).c_str());
    std::wstring detail = loc::ScanSummary(files, categories);
    SetWindowTextW(g_foundDetail, detail.c_str());
    InvalidateRect(g_main, &g_foundCard, FALSE);
}

void UpdateSelectedUi() {
    if (g_showCleanupSummary) {
        SetWindowTextW(g_cleanButton, loc::ui::CleanUp);
        EnableWindow(g_cleanButton, FALSE);
        InvalidateRect(g_main, &g_selectedCard, FALSE);
        return;
    }
    uint64_t bytes = 0;
    int selected = 0;
    bool hasSystemOperation = false;
    for (const auto& item : g_items) {
        if (SendMessageW(item.checkbox, BM_GETCHECK, 0, 0) != BST_CHECKED) continue;
        ++selected;
        AddBytesClamped(bytes, item.scan.bytes);
        if (item.action == ActionType::DismCleanup) hasSystemOperation = true;
    }

    SetWindowTextW(g_selectedValue, selected ? FormatBytes(bytes).c_str() : loc::format::ZeroBytes);
    std::wstring detail = loc::SelectionSummary(selected, hasSystemOperation);
    SetWindowTextW(g_selectedDetail, detail.c_str());

    std::wstring cleanText = loc::ui::CleanUp.str();
    if (selected && g_hasScanned && bytes > 0) cleanText += L" · " + FormatBytes(bytes);
    SetWindowTextW(g_cleanButton, cleanText.c_str());

    const bool idle = g_phase != AppPhase::Scanning && g_phase != AppPhase::Cleaning && g_phase != AppPhase::Cancelling;
    EnableWindow(g_cleanButton, idle && g_hasScanned && selected > 0);
    InvalidateRect(g_main, &g_selectedCard, FALSE);
}

void SetPreset(int id) {
    if (g_busy.load()) return;
    for (auto& item : g_items) {
        bool checked = false;
        if (id == ID_PRESET_RECOMMENDED) checked = item.defaultChecked;
        else if (id == ID_PRESET_SAFE) checked = item.risk == RiskLevel::Safe;
        SendMessageW(item.checkbox, BM_SETCHECK, checked ? BST_CHECKED : BST_UNCHECKED, 0);
    }
    UpdateSelectedUi();
    AppendLogUi(id == ID_PRESET_RECOMMENDED ? loc::ui::RecommendedApplied
                 : id == ID_PRESET_SAFE ? loc::ui::AllSafeSelected
                                        : loc::ui::SelectionCleared);
}

void ApplyUiState(AppPhase phase) {
    g_phase = phase;
    const bool busy = phase == AppPhase::Scanning || phase == AppPhase::Cleaning || phase == AppPhase::Cancelling;
    const bool cancelling = phase == AppPhase::Cancelling;

    for (auto& item : g_items) EnableWindow(item.checkbox, !busy);
    for (HWND control : {g_recommendedButton, g_safeButton, g_clearSelectionButton, g_protectRecentCheck}) {
        EnableWindow(control, !busy);
    }
    EnableWindow(g_storageButton, !busy);
    EnableWindow(g_languageButton, !busy);
    EnableWindow(g_scanButton, !busy);
    EnableWindow(g_stopButton, busy && !cancelling);
    ShowWindow(g_stopButton, busy ? SW_SHOW : SW_HIDE);
    ShowWindow(g_scanButton, busy ? SW_HIDE : SW_SHOW);
    ShowWindow(g_cleanButton, busy ? SW_HIDE : SW_SHOW);

    SetWindowTextW(g_stopButton, cancelling ? loc::ui::Stopping : loc::ui::Stop);
    SetWindowTextW(g_scanButton, g_hasScanned ? loc::ui::ScanAgain : loc::ui::Scan);
    UpdateSelectedUi();
    LayoutUi();
}

void PositionControl(HWND control, int x, int y, int width, int height) {
    if (control) SetWindowPos(control, nullptr, x, y, (std::max)(0, width), (std::max)(0, height),
                              SWP_NOZORDER | SWP_NOACTIVATE);
}

void LayoutUi() {
    if (!g_main) return;
    RECT client{};
    GetClientRect(g_main, &client);
    const int width = client.right;
    const int height = client.bottom;
    if (width <= 0 || height <= 0) return;

    const int margin = Dip(24);
    const int gap = Dip(14);
    const int headerTop = Dip(18);
    const int cardsTop = Dip(92);
    const int cardHeight = Dip(108);
    const int cardWidth = (width - margin * 2 - gap * 2) / 3;

    const int storageWidth = Dip(188);
    const int languageWidth = Dip(132);
    const int versionWidth = Dip(168);
    const int headerGap = Dip(10);
    const int storageX = width - margin - storageWidth;
    const int languageX = storageX - headerGap - languageWidth;
    const int versionRight = languageX - headerGap;
    const int versionLeft = versionRight - versionWidth;
    const int titleLeft = margin + Dip(50);
    const int titleWidth = (std::max)(Dip(260), versionLeft - headerGap - titleLeft);

    PositionControl(g_title, titleLeft, headerTop, titleWidth, Dip(38));
    PositionControl(g_subtitle, margin + Dip(52), headerTop + Dip(38), titleWidth - Dip(2), Dip(24));
    PositionControl(g_languageButton, languageX, headerTop + Dip(10), languageWidth, Dip(42));
    PositionControl(g_storageButton, storageX, headerTop + Dip(10), storageWidth, Dip(42));

    g_freeCard = {margin, cardsTop, margin + cardWidth, cardsTop + cardHeight};
    g_foundCard = {g_freeCard.right + gap, cardsTop, g_freeCard.right + gap + cardWidth, cardsTop + cardHeight};
    g_selectedCard = {g_foundCard.right + gap, cardsTop, width - margin, cardsTop + cardHeight};

    auto layoutSummary = [&](const RECT& card, HWND caption, HWND value, HWND detail) {
        PositionControl(caption, card.left + Dip(18), card.top + Dip(14), card.right - card.left - Dip(36), Dip(18));
        PositionControl(value, card.left + Dip(18), card.top + Dip(35), card.right - card.left - Dip(36), Dip(34));
        PositionControl(detail, card.left + Dip(18), card.top + Dip(72), card.right - card.left - Dip(36), Dip(20));
    };
    layoutSummary(g_freeCard, g_freeCaption, g_freeValue, g_freeDetail);
    layoutSummary(g_foundCard, g_foundCaption, g_foundValue, g_foundDetail);
    layoutSummary(g_selectedCard, g_selectedCaption, g_selectedValue, g_selectedDetail);

    const int contentTop = cardsTop + cardHeight + gap;
    const int footerHeight = Dip(88);
    const int footerTop = height - footerHeight;
    const int contentBottom = footerTop - gap;
    const int rightWidth = (std::min)(Dip(380), (std::max)(Dip(320), width * 31 / 100));
    const int leftWidth = width - margin * 2 - gap - rightWidth;

    g_itemsPanel = {margin, contentTop, margin + leftWidth, contentBottom};
    g_logPanel = {g_itemsPanel.right + gap, contentTop, width - margin, contentBottom};
    g_footerRect = {0, footerTop, width, height};

    PositionControl(g_itemsTitle, g_itemsPanel.left + Dip(18), g_itemsPanel.top + Dip(14), Dip(220), Dip(24));
    PositionControl(g_itemsHint, g_itemsPanel.left + Dip(18), g_itemsPanel.top + Dip(39), Dip(290), Dip(18));

    const int presetY = g_itemsPanel.top + Dip(13);
    const int clearWidth = Dip(76);
    const int safeWidth = Dip(104);
    const int recommendWidth = Dip(132);
    int presetX = g_itemsPanel.right - Dip(16) - clearWidth;
    PositionControl(g_clearSelectionButton, presetX, presetY, clearWidth, Dip(34));
    presetX -= gap / 2 + safeWidth;
    PositionControl(g_safeButton, presetX, presetY, safeWidth, Dip(34));
    presetX -= gap / 2 + recommendWidth;
    PositionControl(g_recommendedButton, presetX, presetY, recommendWidth, Dip(34));
    PositionControl(g_protectRecentCheck, g_itemsPanel.left + Dip(18), g_itemsPanel.top + Dip(57),
                    g_itemsPanel.right - g_itemsPanel.left - Dip(36), Dip(22));

    const int itemTop = g_itemsPanel.top + Dip(86);
    const int itemBottom = g_itemsPanel.bottom - Dip(12);
    const int itemGap = Dip(8);
    const int columns = 2;
    const int rows = static_cast<int>((g_items.size() + columns - 1) / columns);
    const int itemWidth = (g_itemsPanel.right - g_itemsPanel.left - Dip(36) - itemGap) / columns;
    int itemHeight = rows > 0 ? (itemBottom - itemTop - itemGap * (rows - 1)) / rows : 0;
    itemHeight = (std::max)(Dip(48), (std::min)(Dip(64), itemHeight));

    for (size_t index = 0; index < g_items.size(); ++index) {
        const int column = static_cast<int>(index % columns);
        const int row = static_cast<int>(index / columns);
        const int x = g_itemsPanel.left + Dip(18) + column * (itemWidth + itemGap);
        const int y = itemTop + row * (itemHeight + itemGap);
        PositionControl(g_items[index].checkbox, x, y, itemWidth, itemHeight);
    }

    PositionControl(g_logTitle, g_logPanel.left + Dip(16), g_logPanel.top + Dip(14), Dip(160), Dip(24));
    PositionControl(g_logHint, g_logPanel.left + Dip(16), g_logPanel.top + Dip(39), Dip(190), Dip(18));
    PositionControl(g_clearLogButton, g_logPanel.right - Dip(16) - Dip(78), g_logPanel.top + Dip(13), Dip(78), Dip(34));
    PositionControl(g_exportButton, g_logPanel.right - Dip(16) - Dip(78) - Dip(8) - Dip(94),
                    g_logPanel.top + Dip(13), Dip(94), Dip(34));
    PositionControl(g_log, g_logPanel.left + Dip(12), g_logPanel.top + Dip(68),
                    g_logPanel.right - g_logPanel.left - Dip(24),
                    g_logPanel.bottom - g_logPanel.top - Dip(80));

    PositionControl(g_progress, margin, footerTop + Dip(8), width - margin * 2, Dip(7));
    PositionControl(g_status, margin, footerTop + Dip(25), width / 2, Dip(22));
    PositionControl(g_statusDetail, margin, footerTop + Dip(48), width / 2, Dip(20));

    const int buttonY = footerTop + Dip(26);
    if (g_phase == AppPhase::Scanning || g_phase == AppPhase::Cleaning || g_phase == AppPhase::Cancelling) {
        PositionControl(g_stopButton, width - margin - Dip(154), buttonY, Dip(154), Dip(46));
    } else {
        PositionControl(g_cleanButton, width - margin - Dip(196), buttonY, Dip(196), Dip(46));
        PositionControl(g_scanButton, width - margin - Dip(196) - Dip(10) - Dip(154), buttonY, Dip(154), Dip(46));
    }
    RedrawWindow(g_main, nullptr, nullptr, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void PaintMainWindow(HWND hwnd) {
    PAINTSTRUCT paint{};
    HDC output = BeginPaint(hwnd, &paint);
    RECT client{};
    GetClientRect(hwnd, &client);
    const int width = client.right;
    const int height = client.bottom;

    HDC dc = CreateCompatibleDC(output);
    HBITMAP bitmap = CreateCompatibleBitmap(output, (std::max)(1, width), (std::max)(1, height));
    const auto oldBitmap = SelectObject(dc, bitmap);
    FillRect(dc, &client, g_bgBrush);

    RECT logo{Dip(24), Dip(22), Dip(62), Dip(60)};
    DrawRoundedPanel(dc, logo, COLOR_ACCENT_2, COLOR_ACCENT_2, 11);
    HPEN sparklePen = CreatePen(PS_SOLID, Dip(2), RGB(255, 255, 255));
    const auto oldPen = SelectObject(dc, sparklePen);
    const int cx = (logo.left + logo.right) / 2;
    const int cy = (logo.top + logo.bottom) / 2;
    MoveToEx(dc, cx, logo.top + Dip(8), nullptr); LineTo(dc, cx, logo.bottom - Dip(8));
    MoveToEx(dc, logo.left + Dip(8), cy, nullptr); LineTo(dc, logo.right - Dip(8), cy);
    SelectObject(dc, oldPen);
    DeleteObject(sparklePen);

    const int paintMargin = Dip(24);
    const int paintStorageWidth = Dip(188);
    const int paintLanguageWidth = Dip(132);
    const int paintVersionWidth = Dip(168);
    const int paintHeaderGap = Dip(10);
    const int paintStorageX = client.right - paintMargin - paintStorageWidth;
    const int paintLanguageX = paintStorageX - paintHeaderGap - paintLanguageWidth;
    const int paintVersionRight = paintLanguageX - paintHeaderGap;
    RECT version{paintVersionRight - paintVersionWidth, Dip(28), paintVersionRight, Dip(51)};
    DrawRoundedPanel(dc, version, COLOR_PANEL_2, COLOR_BORDER, 12);
    DrawTextStyled(dc, loc::app::AdminBadge, version, g_fontTiny, COLOR_GREEN,
                   DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    DrawRoundedPanel(dc, g_freeCard, COLOR_PANEL, COLOR_BORDER, 16);
    DrawRoundedPanel(dc, g_foundCard, COLOR_PANEL, COLOR_BORDER, 16);
    DrawRoundedPanel(dc, g_selectedCard, COLOR_PANEL, COLOR_BORDER, 16);
    DrawRoundedPanel(dc, g_itemsPanel, COLOR_PANEL, COLOR_BORDER, 18);
    DrawRoundedPanel(dc, g_logPanel, COLOR_PANEL, COLOR_BORDER, 18);

    const DriveStats drive = QueryDriveStats();
    RECT bar{g_freeCard.left + Dip(18), g_freeCard.bottom - Dip(12), g_freeCard.right - Dip(18), g_freeCard.bottom - Dip(7)};
    DrawRoundedPanel(dc, bar, COLOR_PANEL_2, COLOR_PANEL_2, 4);
    if (drive.valid && drive.total) {
        const uint64_t used = drive.total > drive.free ? drive.total - drive.free : 0;
        RECT filled = bar;
        filled.right = filled.left + static_cast<int>((bar.right - bar.left) *
            static_cast<long double>(used) / static_cast<long double>(drive.total));
        if (filled.right > filled.left) DrawRoundedPanel(dc, filled, COLOR_ACCENT, COLOR_ACCENT, 4);
    }

    RECT foundMark{g_foundCard.left, g_foundCard.top + Dip(18), g_foundCard.left + Dip(4), g_foundCard.bottom - Dip(18)};
    HBRUSH foundBrush = CreateSolidBrush(COLOR_GREEN);
    FillRect(dc, &foundMark, foundBrush);
    DeleteObject(foundBrush);
    RECT selectedMark{g_selectedCard.left, g_selectedCard.top + Dip(18), g_selectedCard.left + Dip(4), g_selectedCard.bottom - Dip(18)};
    const COLORREF selectedColor = g_showCleanupSummary && g_lastCleanupStats.errors > 0 ? COLOR_RED :
        (g_showCleanupSummary && g_lastCleanupStats.skippedFiles > 0 ? COLOR_YELLOW : COLOR_ACCENT_2);
    HBRUSH selectedBrush = CreateSolidBrush(selectedColor);
    FillRect(dc, &selectedMark, selectedBrush);
    DeleteObject(selectedBrush);

    HPEN divider = CreatePen(PS_SOLID, 1, COLOR_BORDER);
    const auto oldDivider = SelectObject(dc, divider);
    MoveToEx(dc, 0, g_footerRect.top, nullptr);
    LineTo(dc, client.right, g_footerRect.top);
    SelectObject(dc, oldDivider);
    DeleteObject(divider);

    COLORREF statusColor = COLOR_GREEN;
    if (g_phase == AppPhase::Scanning || g_phase == AppPhase::Cleaning) statusColor = COLOR_ACCENT;
    else if (g_phase == AppPhase::Cancelling || g_phase == AppPhase::CompletedWithSkips) statusColor = COLOR_YELLOW;
    else if (g_phase == AppPhase::PartialFailure) statusColor = COLOR_RED;
    HBRUSH statusBrush = CreateSolidBrush(statusColor);
    HBRUSH oldBrush = reinterpret_cast<HBRUSH>(SelectObject(dc, statusBrush));
    Ellipse(dc, Dip(13), g_footerRect.top + Dip(31), Dip(21), g_footerRect.top + Dip(39));
    SelectObject(dc, oldBrush);
    DeleteObject(statusBrush);

    BitBlt(output, 0, 0, width, height, dc, 0, 0, SRCCOPY);
    SelectObject(dc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(dc);
    EndPaint(hwnd, &paint);
}

bool CreateUi(HWND hwnd) {
    BOOL dark = TRUE;
    DwmSetWindowAttribute(hwnd, 20, &dark, sizeof(dark));
    DwmSetWindowAttribute(hwnd, 19, &dark, sizeof(dark));
    int cornerPreference = 2;
    DwmSetWindowAttribute(hwnd, 33, &cornerPreference, sizeof(cornerPreference));

    g_title = MakeStatic(hwnd, loc::app::Name, g_fontHero);
    g_subtitle = MakeStatic(hwnd, loc::ui::Subtitle, g_fontNormal);
    const std::wstring languageText = LanguageButtonText();
    g_languageButton = MakeButton(hwnd, languageText.c_str(), ID_LANGUAGE, g_fontSmall);
    g_storageButton = MakeButton(hwnd, loc::ui::WindowsStorage, ID_STORAGE_SETTINGS, g_fontSmall);

    g_freeCaption = MakeStatic(hwnd, loc::ui::SystemDrive, g_fontSmall);
    g_freeValue = MakeStatic(hwnd, loc::format::NotAvailable, g_fontValue);
    g_freeDetail = MakeStatic(hwnd, loc::ui::Loading, g_fontSmall);
    g_foundCaption = MakeStatic(hwnd, loc::ui::ScanResults, g_fontSmall);
    g_foundValue = MakeStatic(hwnd, loc::format::ZeroBytes, g_fontValue);
    g_foundDetail = MakeStatic(hwnd, loc::ui::ScanNotRun, g_fontSmall);
    g_selectedCaption = MakeStatic(hwnd, loc::ui::SelectedForCleanup, g_fontSmall);
    g_selectedValue = MakeStatic(hwnd, loc::format::ZeroBytes, g_fontValue);
    g_selectedDetail = MakeStatic(hwnd, loc::ui::NothingSelected, g_fontSmall);

    g_itemsTitle = MakeStatic(hwnd, loc::ui::ItemsToClean, g_fontSubtitle);
    g_itemsHint = MakeStatic(hwnd, loc::ui::ItemsHint, g_fontSmall);
    g_recommendedButton = MakeButton(hwnd, loc::ui::Recommended, ID_PRESET_RECOMMENDED, g_fontSmall);
    g_safeButton = MakeButton(hwnd, loc::ui::AllSafe, ID_PRESET_SAFE, g_fontSmall);
    g_clearSelectionButton = MakeButton(hwnd, loc::ui::Reset, ID_PRESET_CLEAR, g_fontSmall);
    g_protectRecentCheck = CreateWindowExW(
        0, L"BUTTON", loc::ui::ProtectRecent,
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
        0, 0, 0, 0, hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_PROTECT_RECENT)), g_instance, nullptr
    );
    SetControlFont(g_protectRecentCheck, g_fontSmall);
    SetWindowTheme(g_protectRecentCheck, L"", L"");
    SetWindowSubclass(g_protectRecentCheck, ProtectRecentSubclass, 1, 0);
    SendMessageW(g_protectRecentCheck, BM_SETCHECK, BST_CHECKED, 0);

    BuildItems(hwnd);

    g_logTitle = MakeStatic(hwnd, loc::ui::Activity, g_fontSubtitle);
    g_logHint = MakeStatic(hwnd, loc::ui::ActivityHint, g_fontSmall);
    g_exportButton = MakeButton(hwnd, loc::ui::ExportTxt, ID_EXPORT, g_fontSmall);
    g_clearLogButton = MakeButton(hwnd, loc::ui::Clear, ID_CLEAR_LOG, g_fontSmall);

    if (!LoadLibraryW(L"Msftedit.dll")) return false;
    g_log = CreateWindowExW(
        0, MSFTEDIT_CLASS, L"",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
        0, 0, 0, 0, hwnd, nullptr, g_instance, nullptr
    );
    if (!g_log) return false;
    SendMessageW(g_log, EM_SETBKGNDCOLOR, 0, COLOR_PANEL_2);
    SendMessageW(g_log, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(Dip(10), Dip(10)));
    CHARFORMAT2W logFormat{};
    logFormat.cbSize = sizeof(logFormat);
    logFormat.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE;
    logFormat.crTextColor = COLOR_TEXT;
    logFormat.yHeight = 190;
    wcscpy_s(logFormat.szFaceName, L"Cascadia Mono");
    SendMessageW(g_log, EM_SETCHARFORMAT, SCF_ALL, reinterpret_cast<LPARAM>(&logFormat));

    g_progress = CreateWindowExW(
        0, PROGRESS_CLASSW, L"",
        WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
        0, 0, 0, 0, hwnd, nullptr, g_instance, nullptr
    );
    SetWindowTheme(g_progress, L"", L"");
    SetWindowSubclass(g_progress, ProgressSubclass, 1, 0);
    SendMessageW(g_progress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessageW(g_progress, PBM_SETBARCOLOR, 0, COLOR_ACCENT);
    SendMessageW(g_progress, PBM_SETBKCOLOR, 0, COLOR_PANEL_2);

    g_status = MakeStatic(hwnd, loc::ui::ReadyToScan, g_fontSubtitle, SS_LEFT, false);
    g_statusDetail = MakeStatic(hwnd, loc::ui::RecommendedSelected, g_fontSmall, SS_LEFT, false);
    g_scanButton = MakeButton(hwnd, loc::ui::Scan, ID_SCAN, g_fontBold);
    g_cleanButton = MakeButton(hwnd, loc::ui::CleanUp, ID_CLEAN, g_fontBold);
    g_stopButton = MakeButton(hwnd, loc::ui::Stop, ID_STOP, g_fontBold);

    ApplyFonts();
    UpdateDriveUi();
    UpdateFoundUi();
    UpdateSelectedUi();
    ApplyUiState(AppPhase::ReadyUnscanned);
    AppendLogUi(loc::app::StartupReady, LogSeverity::Success);
    return true;
}

std::vector<OperationTask> SelectedTasks() {
    std::vector<OperationTask> tasks;
    for (size_t index = 0; index < g_items.size(); ++index) {
        const auto& item = g_items[index];
        if (SendMessageW(item.checkbox, BM_GETCHECK, 0, 0) == BST_CHECKED) {
            tasks.push_back({static_cast<int>(index), item.action, item.title, item.risk,
                             item.scan.bytes, item.scan.files});
        }
    }
    return tasks;
}

std::set<std::wstring> RunningProcesses() {
    std::set<std::wstring> result;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return result;

    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);
    if (Process32FirstW(snapshot, &entry)) {
        do {
            result.insert(LowerCopy(entry.szExeFile));
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return result;
}

std::vector<std::wstring> CacheAppsStillRunning(const std::vector<OperationTask>& tasks) {
    bool checkBrowsers = false;
    bool checkMessengers = false;
    for (const auto& task : tasks) {
        checkBrowsers = checkBrowsers || task.action == ActionType::BrowserCaches;
        checkMessengers = checkMessengers || task.action == ActionType::CommunicationCaches;
    }
    if (!checkBrowsers && !checkMessengers) return {};

    const auto running = RunningProcesses();
    std::vector<std::wstring> found;
    auto addIfRunning = [&](const wchar_t* executable, const wchar_t* displayName) {
        if (running.find(LowerCopy(executable)) != running.end() &&
            std::find(found.begin(), found.end(), displayName) == found.end()) {
            found.emplace_back(displayName);
        }
    };

    if (checkBrowsers) {
        addIfRunning(L"chrome.exe", loc::process::GoogleChrome);
        addIfRunning(L"msedge.exe", loc::process::MicrosoftEdge);
        addIfRunning(L"brave.exe", loc::process::Brave);
        addIfRunning(L"vivaldi.exe", loc::process::Vivaldi);
        addIfRunning(L"opera.exe", loc::process::Opera);
        addIfRunning(L"firefox.exe", loc::process::Firefox);
    }
    if (checkMessengers) {
        addIfRunning(L"discord.exe", loc::process::Discord);
        addIfRunning(L"slack.exe", loc::process::Slack);
        addIfRunning(L"teams.exe", loc::process::Teams);
        addIfRunning(L"ms-teams.exe", loc::process::NewTeams);
    }
    return found;
}

std::wstring GetLogText() {
    const int length = GetWindowTextLengthW(g_log);
    std::wstring text(static_cast<size_t>((std::max)(0, length)), L'\0');
    if (length > 0) {
        std::vector<wchar_t> buffer(static_cast<size_t>(length) + 1);
        GetWindowTextW(g_log, buffer.data(), static_cast<int>(buffer.size()));
        text.assign(buffer.data(), static_cast<size_t>(length));
    }
    return text;
}

std::wstring BuildReportText() {
    SYSTEMTIME time{};
    GetLocalTime(&time);
    const DriveStats drive = QueryDriveStats();

    std::wostringstream report;
    report << loc::report::Title << L"\r\n"
           << L"=====================================\r\n"
           << std::setfill(L'0') << std::setw(4) << time.wYear << L"-"
           << std::setw(2) << time.wMonth << L"-" << std::setw(2) << time.wDay << L"  "
           << std::setw(2) << time.wHour << L":" << std::setw(2) << time.wMinute << L":" << std::setw(2) << time.wSecond
           << L"\r\n\r\n";

    if (drive.valid) {
        report << loc::report::SystemDrive << SystemDriveLetter() << L":\r\n"
               << loc::report::FreeSpace << FormatBytes(drive.free) << loc::report::Of << FormatBytes(drive.total) << L"\r\n";
    }
    if (g_hasLastCleanup) {
        report << L"\r\n" << loc::report::LastCleanup << L"\r\n"
               << L"------------------\r\n"
               << loc::report::FilesDeleted << g_lastCleanupStats.deletedFiles << L"\r\n"
               << loc::report::SpaceFreed << FormatMeasuredBytes(g_lastFreedBytes, g_lastFreedBytesMeasured) << L"\r\n"
               << loc::report::FilesSkipped << g_lastCleanupStats.skippedFiles << L"\r\n"
               << loc::report::Errors << g_lastCleanupStats.errors << L"\r\n";
    }
    report << loc::report::RecentProtection
           << (g_protectRecent.load() ? loc::report::Enabled : loc::report::Disabled) << L"\r\n\r\n";

    report << loc::report::Categories << L"\r\n----------\r\n";
    for (const auto& item : g_items) {
        const bool checked = SendMessageW(item.checkbox, BM_GETCHECK, 0, 0) == BST_CHECKED;
        report << (checked ? L"[x] " : L"[ ] ") << item.title << L" — " << ItemSizeText(item);
        if (item.scanned) {
            report << L" · " << loc::Files(item.scan.files);
            if (item.scan.skipped) report << L" · " << loc::FilesSkipped(item.scan.skipped);
            if (item.scan.errors) report << L" · " << loc::Errors(item.scan.errors);
        }
        report << L"\r\n";
    }

    report << L"\r\n" << loc::report::ActivityLog << L"\r\n------------\r\n" << GetLogText();
    return report.str();
}

bool WriteUtf8File(const std::wstring& path, const std::wstring& content) {
    const int needed = WideCharToMultiByte(CP_UTF8, 0, content.data(), static_cast<int>(content.size()),
                                           nullptr, 0, nullptr, nullptr);
    if (needed < 0) return false;
    std::vector<char> bytes(static_cast<size_t>(needed));
    if (needed > 0 && WideCharToMultiByte(CP_UTF8, 0, content.data(), static_cast<int>(content.size()),
                                          bytes.data(), needed, nullptr, nullptr) == 0) {
        return false;
    }

    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;

    const BYTE bom[] = {0xEF, 0xBB, 0xBF};
    DWORD written = 0;
    bool ok = WriteFile(file, bom, sizeof(bom), &written, nullptr) && written == sizeof(bom);
    size_t offset = 0;
    while (ok && offset < bytes.size()) {
        const DWORD chunk = static_cast<DWORD>((std::min)(bytes.size() - offset,
            static_cast<size_t>((std::numeric_limits<DWORD>::max)())));
        written = 0;
        ok = WriteFile(file, bytes.data() + offset, chunk, &written, nullptr) && written == chunk;
        offset += written;
    }
    CloseHandle(file);
    return ok;
}

void ExportReport() {
    SYSTEMTIME time{};
    GetLocalTime(&time);
    wchar_t fileName[MAX_PATH]{};
    swprintf_s(fileName, loc::report::FilenamePattern,
               time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);

    std::wstring initialDirectory;
    PWSTR documents = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documents))) {
        initialDirectory = documents;
        CoTaskMemFree(documents);
    }

    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = g_main;
    dialog.lpstrFilter = loc::dialog::TextFilter;
    dialog.lpstrFile = fileName;
    dialog.nMaxFile = static_cast<DWORD>(std::size(fileName));
    dialog.lpstrDefExt = L"txt";
    dialog.lpstrInitialDir = initialDirectory.empty() ? nullptr : initialDirectory.c_str();
    dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
    if (!GetSaveFileNameW(&dialog)) return;

    if (WriteUtf8File(fileName, BuildReportText())) {
        AppendLogUi(loc::dialog::ReportExported + std::wstring(fileName), LogSeverity::Success);
        MessageBoxW(g_main, loc::dialog::ReportSaved, loc::dialog::ExportComplete, MB_OK | MB_ICONINFORMATION);
    } else {
        AppendLogUi(loc::dialog::ReportSaveFailedLog, LogSeverity::Error);
        MessageBoxW(g_main, loc::dialog::ReportSaveFailed, loc::dialog::Error, MB_OK | MB_ICONERROR);
    }
}

void OpenStorageSettings() {
    const auto result = reinterpret_cast<INT_PTR>(ShellExecuteW(g_main, L"open", L"ms-settings:storagesense",
                                                               nullptr, nullptr, SW_SHOWNORMAL));
    if (result <= 32) AppendLogUi(loc::dialog::StorageOpenFailed, LogSeverity::Error);
}

void PostActiveItem(int index, uint64_t session) {
    PostMessageW(g_main, WM_APP_ACTIVE_ITEM, static_cast<WPARAM>(index + 1), static_cast<LPARAM>(session));
}

void StartScan() {
    if (g_busy.exchange(true)) return;
    g_cancel.store(false);
    g_protectRecent.store(SendMessageW(g_protectRecentCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
    const uint64_t session = ++g_sessionCounter;
    g_currentSession = session;

    g_showCleanupSummary = false;
    RestoreScanSummaryUi();
    ResetScanResults();
    UpdateFoundUi();
    UpdateSelectedUi();
    SendMessageW(g_progress, PBM_SETPOS, 0, 0);
    ApplyUiState(AppPhase::Scanning);
    SetStatusText(loc::scan::Status, loc::scan::Detail);
    AppendLogUi(loc::scan::Started);

    std::vector<OperationTask> tasks;
    tasks.reserve(g_items.size());
    for (size_t index = 0; index < g_items.size(); ++index) {
        const auto& item = g_items[index];
        tasks.push_back({static_cast<int>(index), item.action, item.title, item.risk, 0});
    }
    const bool protectRecent = g_protectRecent.load();

    try {
        std::thread([tasks = std::move(tasks), protectRecent, session] {
            bool workerFailed = false;
            try {
                const int count = static_cast<int>(tasks.size());
                for (int step = 0; step < count && !g_cancel.load(); ++step) {
                    const auto& task = tasks[step];
                    PostActiveItem(task.index, session);
                    PostStatus(loc::ProgressStatus(loc::scan::ActiveVerb,
                                                   static_cast<uint64_t>(step + 1),
                                                   static_cast<uint64_t>(count), task.title));

                    ScanStats stats;
                    if (task.action != ActionType::DismCleanup) {
                        stats = ScanPaths(PathsForAction(task.action), protectRecent && ProtectRecentForAction(task.action));
                    }
                    PostOwned(WM_APP_SIZE_RESULT, 0, new SizeResult{task.index, stats, session});
                    PostMessageW(g_main, WM_APP_PROGRESS,
                                 static_cast<WPARAM>(((step + 1) * 100) / (std::max)(1, count)),
                                 static_cast<LPARAM>(session));
                }
            } catch (...) {
                workerFailed = true;
                PostLog(loc::scan::UnexpectedWorkerError, LogSeverity::Error);
            }
            PostActiveItem(-1, session);
            auto* finish = new FinishResult{};
            finish->cancelled = g_cancel.load();
            finish->session = session;
            finish->cleanup.errors = workerFailed ? 1 : 0;
            PostOwned(WM_APP_FINISHED, 0, finish);
        }).detach();
    } catch (...) {
        g_busy.store(false);
        ApplyUiState(AppPhase::PartialFailure);
        SetStatusText(loc::scan::StartFailed, loc::scan::ThreadCreateFailedDetail);
        AppendLogUi(loc::scan::ThreadCreateFailedLog, LogSeverity::Error);
    }
}

std::wstring CleanupConfirmation(const std::vector<OperationTask>& tasks,
                                 const std::vector<std::wstring>& runningApps) {
    uint64_t estimate = 0;
    bool hasCaution = false;
    bool hasSystem = false;
    std::wostringstream text;
    text << loc::CleanupItems(tasks.size()) << loc::confirmation::SelectedSuffix;
    for (const auto& task : tasks) {
        AddBytesClamped(estimate, task.estimatedBytes);
        hasCaution = hasCaution || task.risk == RiskLevel::Caution;
        hasSystem = hasSystem || task.risk == RiskLevel::System;
    }
    text << loc::confirmation::EstimatePrefix << FormatBytes(estimate) << L".\n\n";

    const size_t shown = (std::min)(tasks.size(), static_cast<size_t>(8));
    for (size_t index = 0; index < shown; ++index) text << L"• " << tasks[index].title << L"\n";
    if (tasks.size() > shown) text << loc::confirmation::MorePrefix << (tasks.size() - shown) << loc::confirmation::MoreSuffix;

    if (hasCaution) text << loc::confirmation::Caution;
    if (hasSystem) text << loc::confirmation::System;
    if (!runningApps.empty()) {
        text << loc::confirmation::RunningApps;
        for (const auto& app : runningApps) text << L"• " << app << L"\n";
        text << loc::confirmation::CloseApps;
    }
    text << loc::confirmation::Continue;
    return text.str();
}

void StartCleanup() {
    if (g_busy.load()) return;
    if (!g_hasScanned) {
        MessageBoxW(g_main, loc::dialog::RunScanFirst,
                    loc::dialog::ScanRequired, MB_OK | MB_ICONINFORMATION);
        return;
    }

    auto tasks = SelectedTasks();
    if (tasks.empty()) {
        MessageBoxW(g_main, loc::dialog::SelectCategory, loc::dialog::NothingSelected, MB_OK | MB_ICONINFORMATION);
        return;
    }
    const auto runningApps = CacheAppsStillRunning(tasks);
    const bool warning = std::any_of(tasks.begin(), tasks.end(), [](const OperationTask& task) {
        return task.risk != RiskLevel::Safe;
    }) || !runningApps.empty();
    const std::wstring confirmation = CleanupConfirmation(tasks, runningApps);
    if (MessageBoxW(g_main, confirmation.c_str(), loc::dialog::ConfirmCleanup,
                    MB_YESNO | (warning ? MB_ICONWARNING : MB_ICONQUESTION)) != IDYES) {
        return;
    }

    g_busy.store(true);
    g_cancel.store(false);
    const uint64_t session = ++g_sessionCounter;
    g_currentSession = session;
    const bool protectRecent = g_protectRecent.load();
    SendMessageW(g_progress, PBM_SETPOS, 0, 0);
    ApplyUiState(AppPhase::Cleaning);
    SetStatusText(loc::cleanup::InProgress, loc::cleanup::InProgressDetail);
    AppendLogUi(loc::cleanup::Started);

    try {
        std::thread([tasks = std::move(tasks), protectRecent, session] {
            CleanupStats total;
            const DriveStats driveBefore = QueryDriveStats();
            try {
                const int count = static_cast<int>(tasks.size());
                for (int step = 0; step < count && !g_cancel.load(); ++step) {
                    const auto& task = tasks[step];
                    PostActiveItem(task.index, session);
                    PostStatus(loc::ProgressStatus(loc::cleanup::ActiveVerb,
                                                   static_cast<uint64_t>(step + 1),
                                                   static_cast<uint64_t>(count), task.title));

                    const CleanupStats result = ExecuteAction(task.action, protectRecent,
                                                              task.estimatedBytes, task.estimatedFiles);
                    AccumulateCleanupCategory(total, result, task.title);

                    const std::wstring categorySummary = loc::CategoryCleanupResult(
                        result.errors > 0, task.title,
                        result.deletedFiles, FormatBytes(result.deletedBytes),
                        result.skippedFiles, result.errors);
                    const LogSeverity categorySeverity = result.errors > 0
                        ? LogSeverity::Error : LogSeverity::Success;
                    PostLog(categorySummary, categorySeverity);

                    const std::wstring skipBreakdown = CleanupSkipBreakdown(result);
                    if (!skipBreakdown.empty()) {
                        PostLog(loc::cleanup::SkipsPrefix + task.title + L"]: " + skipBreakdown,
                                LogSeverity::Warning);
                    }
                    PostMessageW(g_main, WM_APP_PROGRESS,
                                 static_cast<WPARAM>(((step + 1) * 100) / (std::max)(1, count)),
                                 static_cast<LPARAM>(session));
                }
            } catch (...) {
                CleanupStats fatal;
                RecordCleanupIssue(fatal, CleanupIssueKind::UnexpectedFailure, false,
                                   loc::system::CleanupWorkerThread);
                AccumulateCleanupCategory(total, fatal, loc::system::CleanupSession);
            }
            LogCleanupDiagnostics(total);
            PostActiveItem(-1, session);
            const DriveStats driveAfter = QueryDriveStats();
            auto* finish = new FinishResult{};
            finish->cleanup = total;
            finish->freeSpaceMeasured = driveBefore.valid && driveAfter.valid;
            if (finish->freeSpaceMeasured && driveAfter.free > driveBefore.free) {
                finish->freeSpaceDelta = driveAfter.free - driveBefore.free;
            }
            finish->cancelled = g_cancel.load();
            finish->session = session;
            PostOwned(WM_APP_FINISHED, 1, finish);
        }).detach();
    } catch (...) {
        g_busy.store(false);
        ApplyUiState(AppPhase::PartialFailure);
        SetStatusText(loc::cleanup::StartFailed, loc::cleanup::ThreadCreateFailedDetail);
        AppendLogUi(loc::cleanup::ThreadCreateFailedLog, LogSeverity::Error);
    }
}

void RequestCancellation() {
    if (!g_busy.load() || g_phase == AppPhase::Cancelling) return;
    g_cancel.store(true);
    ApplyUiState(AppPhase::Cancelling);
    SetStatusText(loc::cleanup::Stopping, loc::cleanup::StoppingDetail);
    AppendLogUi(loc::cleanup::StopRequested, LogSeverity::Warning);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            return CreateUi(hwnd) ? 0 : -1;

        case WM_COMMAND: {
            const int id = LOWORD(wParam);
            const int notification = HIWORD(wParam);
            if (id == ID_SCAN && (notification == BN_CLICKED || notification == 1)) {
                StartScan();
                return 0;
            }
            if (id == ID_CLEAN && notification == BN_CLICKED) {
                StartCleanup();
                return 0;
            }
            if (id == ID_STOP && (notification == BN_CLICKED || notification == 1)) {
                RequestCancellation();
                return 0;
            }
            if ((id == ID_PRESET_RECOMMENDED || id == ID_PRESET_SAFE || id == ID_PRESET_CLEAR) &&
                notification == BN_CLICKED) {
                SetPreset(id);
                return 0;
            }
            if (id == ID_LANGUAGE && notification == BN_CLICKED) {
                ShowLanguageMenu(hwnd);
                return 0;
            }
            if (id == ID_EXPORT && (notification == BN_CLICKED || notification == 1)) {
                ExportReport();
                return 0;
            }
            if (id == ID_STORAGE_SETTINGS && notification == BN_CLICKED) {
                OpenStorageSettings();
                return 0;
            }
            if (id == ID_CLEAR_LOG && notification == BN_CLICKED) {
                SetWindowTextW(g_log, L"");
                AppendLogUi(loc::cleanup::LogCleared);
                return 0;
            }
            if (id == ID_PROTECT_RECENT && notification == BN_CLICKED) {
                g_protectRecent.store(SendMessageW(g_protectRecentCheck, BM_GETCHECK, 0, 0) == BST_CHECKED);
                if (g_hasScanned) {
                    ResetScanResults();
                    UpdateFoundUi();
                    ApplyUiState(AppPhase::ReadyUnscanned);
                    SetStatusText(loc::cleanup::RescanRequired, loc::cleanup::ProtectionChanged);
                }
                const bool protectionEnabled = g_protectRecent.load();
                AppendLogUi(protectionEnabled ? loc::cleanup::ProtectionEnabled : loc::cleanup::ProtectionDisabled,
                            protectionEnabled ? LogSeverity::Info : LogSeverity::Warning);
                return 0;
            }
            if (id >= 2000 && id < 2000 + static_cast<int>(g_items.size()) && notification == BN_CLICKED) {
                UpdateSelectedUi();
                return 0;
            }
            break;
        }

        case WM_DRAWITEM:
            return DrawOwnerButton(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));

        case WM_CTLCOLORSTATIC: {
            HDC dc = reinterpret_cast<HDC>(wParam);
            HWND control = reinterpret_cast<HWND>(lParam);
            if (control == g_status || control == g_statusDetail) {
                SetBkMode(dc, OPAQUE);
                SetBkColor(dc, COLOR_BG);
                SetTextColor(dc, control == g_statusDetail ? COLOR_MUTED : COLOR_TEXT);
                return reinterpret_cast<INT_PTR>(g_bgBrush);
            }
            SetBkMode(dc, TRANSPARENT);

            COLORREF color = COLOR_TEXT;
            if (control == g_subtitle || control == g_itemsHint || control == g_logHint ||
                control == g_freeDetail || control == g_foundDetail || control == g_selectedDetail) {
                color = COLOR_MUTED;
            } else if (control == g_freeCaption || control == g_foundCaption || control == g_selectedCaption) {
                color = COLOR_SUBTLE;
            } else if (control == g_foundValue) {
                color = COLOR_GREEN;
            } else if (control == g_selectedValue) {
                color = g_showCleanupSummary && g_lastCleanupStats.errors > 0 ? COLOR_RED :
                    (g_showCleanupSummary && g_lastCleanupStats.skippedFiles > 0 ? COLOR_YELLOW : COLOR_ACCENT_2);
            }
            SetTextColor(dc, color);
            return reinterpret_cast<INT_PTR>(GetStockObject(NULL_BRUSH));
        }

        case WM_CTLCOLORBTN: {
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, COLOR_TEXT);
            SetBkColor(dc, COLOR_PANEL);
            return reinterpret_cast<INT_PTR>(g_panelBrush);
        }

        case WM_CTLCOLOREDIT: {
            HDC dc = reinterpret_cast<HDC>(wParam);
            SetTextColor(dc, COLOR_TEXT);
            SetBkColor(dc, COLOR_PANEL_2);
            return reinterpret_cast<INT_PTR>(g_panelBrush);
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
            PaintMainWindow(hwnd);
            return 0;

        case WM_SIZE:
            LayoutUi();
            return 0;

        case WM_GETMINMAXINFO: {
            auto* info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = Dip(1120);
            info->ptMinTrackSize.y = Dip(780);
            return 0;
        }

        case WM_DPICHANGED: {
            g_dpi = HIWORD(wParam);
            const RECT* suggested = reinterpret_cast<RECT*>(lParam);
            SetWindowPos(hwnd, nullptr, suggested->left, suggested->top,
                         suggested->right - suggested->left, suggested->bottom - suggested->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            CreateFonts();
            ApplyFonts();
            LayoutUi();
            return 0;
        }

        case WM_SETTINGCHANGE:
        case WM_THEMECHANGED:
            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;

        case WM_APP_LOG: {
            std::unique_ptr<std::wstring> text(reinterpret_cast<std::wstring*>(lParam));
            if (text) AppendLogUi(*text, static_cast<LogSeverity>(wParam));
            return 0;
        }

        case WM_APP_STATUS: {
            std::unique_ptr<std::wstring> text(reinterpret_cast<std::wstring*>(lParam));
            if (text) {
                SetWindowTextW(g_status, text->c_str());
                RedrawWindow(g_status, nullptr, nullptr,
                             RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
                NotifyWinEvent(EVENT_OBJECT_NAMECHANGE, g_status, OBJID_CLIENT, CHILDID_SELF);
            }
            return 0;
        }

        case WM_APP_SIZE_RESULT: {
            std::unique_ptr<SizeResult> result(reinterpret_cast<SizeResult*>(lParam));
            if (!result || result->session != g_currentSession) return 0;
            if (result->index >= 0 && result->index < static_cast<int>(g_items.size())) {
                auto& item = g_items[static_cast<size_t>(result->index)];
                item.scan = result->stats;
                item.scanned = true;
                InvalidateRect(item.checkbox, nullptr, FALSE);
                UpdateFoundUi();
                UpdateSelectedUi();
                const std::wstring logLine = loc::ScanItemResult(
                    item.title, FormatBytes(item.scan.bytes), item.scan.files, item.scan.skipped);
                AppendLogUi(logLine);
            }
            return 0;
        }

        case WM_APP_PROGRESS:
            if (static_cast<uint64_t>(lParam) == g_currentSession) {
                SendMessageW(g_progress, PBM_SETPOS, wParam, 0);
            }
            return 0;

        case WM_APP_ACTIVE_ITEM:
            if (static_cast<uint64_t>(lParam) == g_currentSession) {
                const int previous = g_activeItem;
                g_activeItem = static_cast<int>(wParam) - 1;
                if (previous >= 0 && previous < static_cast<int>(g_items.size()))
                    InvalidateRect(g_items[static_cast<size_t>(previous)].checkbox, nullptr, FALSE);
                if (g_activeItem >= 0 && g_activeItem < static_cast<int>(g_items.size()))
                    InvalidateRect(g_items[static_cast<size_t>(g_activeItem)].checkbox, nullptr, FALSE);
            }
            return 0;

        case WM_APP_FINISHED: {
            std::unique_ptr<FinishResult> result(reinterpret_cast<FinishResult*>(lParam));
            if (!result || result->session != g_currentSession) return 0;
            const bool wasCleanup = wParam == 1;
            g_busy.store(false);
            g_activeItem = -1;
            UpdateDriveUi();

            if (!wasCleanup) {
                if (result->cancelled) {
                    g_hasScanned = false;
                    ApplyUiState(AppPhase::ReadyUnscanned);
                    SetStatusText(loc::scan::Stopped, loc::scan::StoppedDetail);
                    AppendLogUi(loc::scan::StoppedLog, LogSeverity::Warning);
                } else if (result->cleanup.errors > 0) {
                    g_hasScanned = false;
                    ApplyUiState(AppPhase::PartialFailure);
                    SetStatusText(loc::scan::Failed, loc::scan::FailedDetail);
                } else {
                    g_hasScanned = true;
                    SendMessageW(g_progress, PBM_SETPOS, 100, 0);
                    ApplyUiState(AppPhase::ScanComplete);
                    SetStatusText(loc::scan::Complete, loc::scan::CompleteDetail);
                    AppendLogUi(loc::scan::CompleteLog, LogSeverity::Success);
                }
                UpdateFoundUi();
                UpdateSelectedUi();
            } else {
                g_lastFreedBytes = result->freeSpaceDelta;
                g_lastFreedBytesMeasured = result->freeSpaceMeasured;
                g_lastCleanupStats = result->cleanup;
                g_hasLastCleanup = true;
                AppendLogUi(loc::cleanup::Summary);
                AppendLogUi(loc::report::FilesDeleted + std::to_wstring(result->cleanup.deletedFiles));
                AppendLogUi(loc::report::SpaceFreed +
                            FormatMeasuredBytes(result->freeSpaceDelta, result->freeSpaceMeasured));
                AppendLogUi(loc::report::FilesSkipped + std::to_wstring(result->cleanup.skippedFiles),
                            result->cleanup.skippedFiles > 0 ? LogSeverity::Warning : LogSeverity::Info);
                AppendLogUi(loc::report::Errors + std::to_wstring(result->cleanup.errors),
                            result->cleanup.errors > 0 ? LogSeverity::Error : LogSeverity::Info);

                ResetScanResults();
                g_showCleanupSummary = true;
                ShowCleanupSummaryUi(result->cleanup, result->freeSpaceDelta, result->freeSpaceMeasured);
                const std::wstring resultDetails = loc::CleanupMetrics(
                    result->cleanup.deletedFiles,
                    FormatMeasuredBytes(result->freeSpaceDelta, result->freeSpaceMeasured),
                    result->cleanup.skippedFiles,
                    result->cleanup.errors);
                if (result->cancelled) {
                    ApplyUiState(AppPhase::PartialFailure);
                    SetStatusText(loc::cleanup::Stopped, resultDetails);
                } else if (result->cleanup.errors > 0) {
                    ApplyUiState(AppPhase::PartialFailure);
                    SetStatusText(loc::cleanup::CompletedErrors, resultDetails);
                } else if (result->cleanup.skippedFiles > 0) {
                    ApplyUiState(AppPhase::CompletedWithSkips);
                    SetStatusText(loc::cleanup::CompletedSkips, resultDetails);
                } else {
                    ApplyUiState(AppPhase::Completed);
                    SetStatusText(loc::cleanup::CompletedSuccess, resultDetails);
                }
            }

            if (g_closeWhenDone) DestroyWindow(hwnd);
            return 0;
        }

        case WM_CLOSE:
            if (g_busy.load()) {
                if (MessageBoxW(hwnd,
                        loc::dialog::OperationRunningPrompt,
                        loc::dialog::OperationInProgress, MB_YESNO | MB_ICONWARNING) == IDYES) {
                    g_closeWhenDone = true;
                    RequestCancellation();
                }
                return 0;
            }
            DestroyWindow(hwnd);
            return 0;

        case WM_DESTROY:
            DeleteFonts();
            if (g_bgBrush) DeleteObject(g_bgBrush);
            if (g_panelBrush) DeleteObject(g_panelBrush);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, message, wParam, lParam);
}

#ifndef N91_DISK_CLEANER_LOGIC_TESTS
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand) {
    g_instance = instance;
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    g_dpi = GetDpiForSystem();

    INITCOMMONCONTROLSEX controls{};
    controls.dwSize = sizeof(controls);
    controls.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
    if (!InitCommonControlsEx(&controls)) return 1;

    CreateFonts();
    g_bgBrush = CreateSolidBrush(COLOR_BG);
    g_panelBrush = CreateSolidBrush(COLOR_PANEL_2);

    const wchar_t className[] = L"N91DiskCleaner.v2";
    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hIcon = LoadIconW(instance, MAKEINTRESOURCEW(1));
    windowClass.hIconSm = LoadIconW(instance, MAKEINTRESOURCEW(1));
    windowClass.hbrBackground = g_bgBrush;
    windowClass.lpszClassName = className;
    windowClass.style = CS_DBLCLKS;
    if (!RegisterClassExW(&windowClass)) return 2;

    const DWORD style = WS_OVERLAPPEDWINDOW;
    const DWORD exStyle = WS_EX_CONTROLPARENT;
    RECT desired{0, 0, Dip(1320), Dip(840)};
    AdjustWindowRectExForDpi(&desired, style, FALSE, exStyle, g_dpi);
    int width = desired.right - desired.left;
    int height = desired.bottom - desired.top;

    RECT workArea{};
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);
    width = (std::min)(width, static_cast<int>(workArea.right - workArea.left));
    height = (std::min)(height, static_cast<int>(workArea.bottom - workArea.top));
    const int x = workArea.left + ((workArea.right - workArea.left) - width) / 2;
    const int y = workArea.top + ((workArea.bottom - workArea.top) - height) / 2;

    g_main = CreateWindowExW(
        exStyle, className, loc::app::WindowTitle,
        style | WS_CLIPCHILDREN,
        x, y, width, height,
        nullptr, nullptr, instance, nullptr
    );
    if (!g_main) return 3;

    ShowWindow(g_main, showCommand);
    UpdateWindow(g_main);

    ACCEL acceleratorEntries[] = {
        {FVIRTKEY, VK_F5, ID_SCAN},
        {FVIRTKEY, VK_ESCAPE, ID_STOP},
        {static_cast<BYTE>(FVIRTKEY | FCONTROL), static_cast<WORD>('S'), ID_EXPORT}
    };
    HACCEL accelerators = CreateAcceleratorTableW(
        acceleratorEntries, static_cast<int>(std::size(acceleratorEntries)));

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        if (accelerators && TranslateAcceleratorW(g_main, accelerators, &message)) continue;
        if (!IsDialogMessageW(g_main, &message)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }
    if (accelerators) DestroyAcceleratorTable(accelerators);
    return static_cast<int>(message.wParam);
}
#endif
