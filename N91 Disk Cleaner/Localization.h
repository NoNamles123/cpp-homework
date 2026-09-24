#pragma once

#include <cstdint>
#include <string>
#include <ostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace localization {

enum class Language : std::uint8_t {
    English,
    Ukrainian,
    Russian,
};

inline Language DetectSystemLanguage() noexcept {
#ifdef _WIN32
    const LANGID languageId = GetUserDefaultUILanguage();
    switch (PRIMARYLANGID(languageId)) {
        case LANG_UKRAINIAN:
            return Language::Ukrainian;
        case LANG_RUSSIAN:
            return Language::Russian;
        default:
            return Language::English;
    }
#else
    return Language::English;
#endif
}

inline Language& CurrentLanguageStorage() noexcept {
    static Language language = DetectSystemLanguage();
    return language;
}

inline Language GetLanguage() noexcept {
    return CurrentLanguageStorage();
}

inline void SetLanguage(Language language) noexcept {
    CurrentLanguageStorage() = language;
}

inline const wchar_t* LanguageCode(Language language) noexcept {
    switch (language) {
        case Language::Ukrainian: return L"uk";
        case Language::Russian: return L"ru";
        default: return L"en";
    }
}

inline const wchar_t* LanguageName(Language language) noexcept {
    switch (language) {
        case Language::Ukrainian: return L"Українська";
        case Language::Russian: return L"Русский";
        default: return L"English";
    }
}

struct LocalizedString {
    const wchar_t* en;
    const wchar_t* uk;
    const wchar_t* ru;

    const wchar_t* c_str() const noexcept {
        switch (GetLanguage()) {
            case Language::Ukrainian: return uk;
            case Language::Russian: return ru;
            default: return en;
        }
    }

    operator const wchar_t*() const noexcept {
        return c_str();
    }

    std::wstring str() const {
        return std::wstring(c_str());
    }
};

struct CategoryText {
    LocalizedString title;
    LocalizedString description;
};

// Make LocalizedString behave like a normal wide string at existing call sites.
// This is important for expressions such as `loc::report::FilesDeleted + std::to_wstring(...)`
// and for streaming localized values into std::wostringstream.
inline std::wstring operator+(const LocalizedString& left, const std::wstring& right) {
    return std::wstring(left.c_str()) + right;
}

inline std::wstring operator+(const std::wstring& left, const LocalizedString& right) {
    return left + right.c_str();
}

inline std::wstring operator+(const LocalizedString& left, const wchar_t* right) {
    return std::wstring(left.c_str()) + (right ? right : L"");
}

inline std::wstring operator+(const wchar_t* left, const LocalizedString& right) {
    return std::wstring(left ? left : L"") + right.c_str();
}

inline std::wstring operator+(const LocalizedString& left, const LocalizedString& right) {
    return std::wstring(left.c_str()) + right.c_str();
}

inline std::wostream& operator<<(std::wostream& stream, const LocalizedString& value) {
    return stream << value.c_str();
}

// Compatibility facade: the project already references localization::english.
// The values below are now runtime-localized, while the namespace name stays the same
// so existing call sites do not need to be rewritten.
namespace english {

namespace app {
inline constexpr LocalizedString Name{L"N91 Disk Cleaner", L"N91 Disk Cleaner", L"N91 Disk Cleaner"};
inline constexpr LocalizedString NameVersion{L"N91 Disk Cleaner 2.0", L"N91 Disk Cleaner 2.0", L"N91 Disk Cleaner 2.0"};
inline constexpr LocalizedString WindowTitle{L"N91 Disk Cleaner 2.0", L"N91 Disk Cleaner 2.0", L"N91 Disk Cleaner 2.0"};
inline constexpr LocalizedString StartupReady{
    L"N91 Disk Cleaner 2.0 is ready. Start with a safe read-only scan.",
    L"N91 Disk Cleaner 2.0 готовий. Почніть із безпечного сканування лише для читання.",
    L"N91 Disk Cleaner 2.0 готов. Начните с безопасного сканирования только для чтения."
};
inline constexpr LocalizedString AdminBadge{L"v2.0  ·  ADMIN MODE", L"v2.0  ·  АДМІН", L"v2.0  ·  АДМИН"};
}

namespace format {
inline constexpr LocalizedString ByteUnits[] = {
    {L"B", L"Б", L"Б"}, {L"KB", L"КБ", L"КБ"}, {L"MB", L"МБ", L"МБ"},
    {L"GB", L"ГБ", L"ГБ"}, {L"TB", L"ТБ", L"ТБ"}
};
inline constexpr LocalizedString NotAvailable{L"—", L"—", L"—"};
inline constexpr LocalizedString ZeroBytes{L"0 B", L"0 Б", L"0 Б"};
}

namespace issue {
inline constexpr LocalizedString RecentFile{L"files modified within the last 24 hours", L"файли, змінені протягом останніх 24 годин", L"файлы, изменённые за последние 24 часа"};
inline constexpr LocalizedString AlreadyGone{L"files or paths that no longer exist", L"файли або шляхи, яких більше не існує", L"файлы или пути, которых больше не существует"};
inline constexpr LocalizedString BusyFile{L"files currently in use by another process", L"файли, які зараз використовує інший процес", L"файлы, которые сейчас используются другим процессом"};
inline constexpr LocalizedString AccessDenied{L"protected files with access denied", L"захищені файли, до яких немає доступу", L"защищённые файлы, к которым нет доступа"};
inline constexpr LocalizedString ReparsePoint{L"symbolic links and reparse-backed items", L"символічні посилання та об’єкти на основі reparse points", L"символические ссылки и объекты на основе reparse points"};
inline constexpr LocalizedString ProtectedOfflineImage{L"protected offline Windows image workspaces", L"захищені робочі області офлайн-образів Windows", L"защищённые рабочие области офлайн-образов Windows"};
inline constexpr LocalizedString UnavailableEntry{L"files unavailable to the system", L"файли, недоступні системі", L"файлы, недоступные системе"};
inline constexpr LocalizedString CancelledOperation{L"cancelled operations", L"скасовані операції", L"отменённые операции"};
inline constexpr LocalizedString UnsupportedEntry{L"unsupported file-system entries", L"непідтримувані об’єкти файлової системи", L"неподдерживаемые объекты файловой системы"};
inline constexpr LocalizedString RetainedDirectory{L"non-empty directories left in place", L"непорожні каталоги, залишені без змін", L"непустые каталоги, оставленные без изменений"};
inline constexpr LocalizedString UnsafeTarget{L"unsafe cleanup target blocked", L"небезпечну ціль очищення заблоковано", L"опасная цель очистки заблокирована"};
inline constexpr LocalizedString MetadataFailure{L"metadata read failure", L"помилка читання метаданих", L"ошибка чтения метаданных"};
inline constexpr LocalizedString EnumerationFailure{L"directory enumeration failure", L"помилка переліку вмісту каталогу", L"ошибка перечисления содержимого каталога"};
inline constexpr LocalizedString FileDeleteFailure{L"file deletion failure", L"помилка видалення файлу", L"ошибка удаления файла"};
inline constexpr LocalizedString DirectoryDeleteFailure{L"directory deletion failure", L"помилка видалення каталогу", L"ошибка удаления каталога"};
inline constexpr LocalizedString ServiceFailure{L"Windows service operation failure", L"помилка операції зі службою Windows", L"ошибка операции со службой Windows"};
inline constexpr LocalizedString SystemToolFailure{L"system command failure", L"помилка системної команди", L"ошибка системной команды"};
inline constexpr LocalizedString RecycleBinFailure{L"Recycle Bin cleanup failure", L"помилка очищення Кошика", L"ошибка очистки Корзины"};
inline constexpr LocalizedString UnexpectedFailure{L"unexpected operation failure", L"неочікувана помилка операції", L"неожиданная ошибка операции"};
inline constexpr LocalizedString Unknown{L"unknown issue type", L"невідомий тип проблеми", L"неизвестный тип проблемы"};
inline constexpr LocalizedString ErrorPrefix{L"Error: ", L"Помилка: ", L"Ошибка: "};
inline constexpr LocalizedString CodePrefix{L" (code ", L" (код ", L" (код "};
inline constexpr LocalizedString CodeSuffix{L")", L")", L")"};
inline constexpr LocalizedString HiddenOne{L" more similar message hidden · ", L" схоже повідомлення приховано · ", L" похожее сообщение скрыто · "};
inline constexpr LocalizedString HiddenMany{L" more similar messages hidden · ", L" схожих повідомлень приховано · ", L" похожих сообщений скрыто · "};
}

namespace skip {
inline constexpr LocalizedString Recent{L"protected (<24h)", L"захищено (<24 год)", L"защищено (<24 ч)"};
inline constexpr LocalizedString Busy{L"in use", L"використовується", L"используется"};
inline constexpr LocalizedString AccessDenied{L"access denied", L"доступ заборонено", L"доступ запрещён"};
inline constexpr LocalizedString AlreadyGone{L"already gone", L"вже відсутнє", L"уже отсутствует"};
inline constexpr LocalizedString ReparsePoint{L"links/reparse items", L"посилання/reparse-об’єкти", L"ссылки/reparse-объекты"};
inline constexpr LocalizedString ProtectedOfflineImage{L"offline Windows images", L"офлайн-образи Windows", L"офлайн-образы Windows"};
inline constexpr LocalizedString UnavailableEntry{L"unavailable", L"недоступне", L"недоступно"};
inline constexpr LocalizedString Other{L"other entries", L"інші об’єкти", L"другие объекты"};
inline constexpr LocalizedString Cancelled{L"cancelled", L"скасовано", L"отменено"};
}

namespace system {
inline constexpr LocalizedString Waiting{L"Waiting for the system operation to finish safely...", L"Очікування безпечного завершення системної операції...", L"Ожидание безопасного завершения системной операции..."};
inline constexpr LocalizedString NotForceStopped{L"The system operation will not be terminated forcibly to avoid damaging Windows.", L"Системну операцію не буде примусово завершено, щоб не пошкодити Windows.", L"Системная операция не будет принудительно завершена, чтобы не повредить Windows."};
inline constexpr LocalizedString ServiceManagerOpenFailed{L"failed to open the Service Control Manager", L"не вдалося відкрити диспетчер керування службами", L"не удалось открыть диспетчер управления службами"};
inline constexpr LocalizedString ServiceOpenFailed{L"failed to open the service", L"не вдалося відкрити службу", L"не удалось открыть службу"};
inline constexpr LocalizedString ServiceStateFailed{L"failed to query the service state", L"не вдалося отримати стан служби", L"не удалось получить состояние службы"};
inline constexpr LocalizedString ServiceStopFailed{L"the service did not stop; this category was skipped", L"служба не зупинилася; цю категорію пропущено", L"служба не остановилась; эта категория пропущена"};
inline constexpr LocalizedString ServiceRestoreFailed{L"failed to restore the service", L"не вдалося відновити службу", L"не удалось восстановить службу"};
inline constexpr LocalizedString DismWarning{L"DISM may take 5–30 minutes. Do not close the application.", L"DISM може працювати 5–30 хвилин. Не закривайте програму.", L"DISM может работать 5–30 минут. Не закрывайте программу."};
inline constexpr LocalizedString UnknownCleanupAction{L"unknown cleanup action", L"невідома дія очищення", L"неизвестное действие очистки"};
inline constexpr LocalizedString CleanupWorkerThread{L"cleanup worker thread", L"робочий потік очищення", L"рабочий поток очистки"};
inline constexpr LocalizedString CleanupSession{L"Cleanup session", L"Сеанс очищення", L"Сеанс очистки"};
inline constexpr LocalizedString DeliveryOptimization{L"Delivery Optimization", L"Оптимізація доставки", L"Оптимизация доставки"};
inline constexpr LocalizedString WindowsUpdate{L"Windows Update", L"Windows Update", L"Windows Update"};
}

namespace risk {
inline constexpr LocalizedString Safe{L"SAFE", L"БЕЗПЕЧНО", L"БЕЗОПАСНО"};
inline constexpr LocalizedString Caution{L"CAUTION", L"ОБЕРЕЖНО", L"ОСТОРОЖНО"};
inline constexpr LocalizedString System{L"SYSTEM", L"СИСТЕМА", L"СИСТЕМА"};
}

namespace item {
inline constexpr LocalizedString NotScanned{L"NOT SCANNED", L"НЕ СКАНОВАНО", L"НЕ СКАНИРОВАНО"};
inline constexpr LocalizedString DismEstimate{L"DISM ESTIMATE", L"ОЦІНКА DISM", L"ОЦЕНКА DISM"};
inline constexpr LocalizedString SomeUnavailable{L"some items unavailable", L"деякі об’єкти недоступні", L"некоторые объекты недоступны"};
}

namespace category {
inline constexpr CategoryText UserTemp{{L"User Temp Files", L"Тимчасові файли користувача", L"Временные файлы пользователя"}, {L"AppData\\Temp; active files are skipped", L"AppData\\Temp; активні файли пропускаються", L"AppData\\Temp; активные файлы пропускаются"}};
inline constexpr CategoryText WindowsTemp{{L"Windows Temp Files", L"Тимчасові файли Windows", L"Временные файлы Windows"}, {L"Temporary files created by Windows", L"Тимчасові файли, створені Windows", L"Временные файлы, созданные Windows"}};
inline constexpr CategoryText ErrorReports{{L"Error Reports", L"Звіти про помилки", L"Отчёты об ошибках"}, {L"Windows Error Reporting archives and queues", L"Архіви та черги Windows Error Reporting", L"Архивы и очереди Windows Error Reporting"}};
inline constexpr CategoryText DeliveryOptimization{{L"Delivery Optimization", L"Оптимізація доставки", L"Оптимизация доставки"}, {L"Cached Windows update delivery files", L"Кешовані файли доставки оновлень Windows", L"Кэшированные файлы доставки обновлений Windows"}};
inline constexpr CategoryText Thumbnails{{L"Thumbnails & Icons", L"Ескізи та значки", L"Эскизы и значки"}, {L"Windows rebuilds them automatically", L"Windows автоматично створить їх знову", L"Windows автоматически создаст их заново"}};
inline constexpr CategoryText Browsers{{L"Browser Caches", L"Кеші браузерів", L"Кэши браузеров"}, {L"Chrome, Edge, Firefox, Brave, and more", L"Chrome, Edge, Firefox, Brave та інші", L"Chrome, Edge, Firefox, Brave и другие"}};
inline constexpr CategoryText Messaging{{L"Messaging Caches", L"Кеші месенджерів", L"Кэши мессенджеров"}, {L"Discord, Slack, and Teams; accounts stay intact", L"Discord, Slack і Teams; облікові записи залишаться без змін", L"Discord, Slack и Teams; учётные записи останутся без изменений"}};
inline constexpr CategoryText Shaders{{L"Shader Caches", L"Кеші шейдерів", L"Кэши шейдеров"}, {L"DirectX, NVIDIA, and AMD; may briefly stutter", L"DirectX, NVIDIA та AMD; після очищення можливі короткі підгальмовування", L"DirectX, NVIDIA и AMD; после очистки возможны краткие подтормаживания"}};
inline constexpr CategoryText CrashDumps{{L"Crash Dumps", L"Дампи збоїв", L"Дампы сбоев"}, {L"MEMORY.DMP, minidumps, and app crash dumps", L"MEMORY.DMP, мінідампи та дампи збоїв програм", L"MEMORY.DMP, минидампы и дампы сбоев программ"}};
inline constexpr CategoryText RecycleBin{{L"Recycle Bin", L"Кошик", L"Корзина"}, {L"Files will be permanently deleted", L"Файли буде видалено назавжди", L"Файлы будут удалены безвозвратно"}};
inline constexpr CategoryText UpdateDownloads{{L"Update Downloads", L"Завантаження оновлень", L"Загрузки обновлений"}, {L"Previously downloaded Windows Update files", L"Раніше завантажені файли Windows Update", L"Ранее загруженные файлы Windows Update"}};
inline constexpr CategoryText ComponentStore{{L"Component Store", L"Сховище компонентів", L"Хранилище компонентов"}, {L"DISM cleanup; may take 5–30 min", L"Очищення DISM; може тривати 5–30 хв", L"Очистка DISM; может занять 5–30 мин"}};
inline constexpr CategoryText DisableHibernation{{L"Disable Hibernation", L"Вимкнути гібернацію", L"Отключить гибернацию"}, {L"Removes hiberfil.sys; disables Fast Startup", L"Видаляє hiberfil.sys і вимикає швидкий запуск", L"Удаляет hiberfil.sys и отключает быстрый запуск"}};
}

namespace noun {
inline constexpr LocalizedString ErrorOne{L"real error", L"реальна помилка", L"реальная ошибка"};
inline constexpr LocalizedString ErrorMany{L"real errors", L"реальних помилок", L"реальных ошибок"};
inline constexpr LocalizedString FileOne{L"file", L"файл", L"файл"};
inline constexpr LocalizedString FileMany{L"files", L"файлів", L"файлов"};
inline constexpr LocalizedString CategoryOne{L"category", L"категорія", L"категория"};
inline constexpr LocalizedString CategoryMany{L"categories", L"категорій", L"категорий"};
inline constexpr LocalizedString ItemOne{L"item", L"об’єкт", L"объект"};
inline constexpr LocalizedString ItemMany{L"items", L"об’єктів", L"объектов"};
inline constexpr LocalizedString CleanupItemOne{L"cleanup item", L"об’єкт очищення", L"объект очистки"};
inline constexpr LocalizedString CleanupItemMany{L"cleanup items", L"об’єктів очищення", L"объектов очистки"};
inline constexpr LocalizedString FileDeletedOne{L"file deleted", L"файл видалено", L"файл удалён"};
inline constexpr LocalizedString FileDeletedMany{L"files deleted", L"файлів видалено", L"файлов удалено"};
inline constexpr LocalizedString FileSkippedOne{L"file skipped", L"файл пропущено", L"файл пропущен"};
inline constexpr LocalizedString FileSkippedMany{L"files skipped", L"файлів пропущено", L"файлов пропущено"};
}

namespace ui {
inline constexpr LocalizedString SystemDrive{L"SYSTEM DRIVE", L"СИСТЕМНИЙ ДИСК", L"СИСТЕМНЫЙ ДИСК"};
inline constexpr LocalizedString UnableDriveStatus{L"Unable to read drive status", L"Не вдалося прочитати стан диска", L"Не удалось прочитать состояние диска"};
inline constexpr LocalizedString ScanResults{L"SCAN RESULTS", L"РЕЗУЛЬТАТИ СКАНУВАННЯ", L"РЕЗУЛЬТАТЫ СКАНИРОВАНИЯ"};
inline constexpr LocalizedString SelectedForCleanup{L"SELECTED FOR CLEANUP", L"ВИБРАНО ДЛЯ ОЧИЩЕННЯ", L"ВЫБРАНО ДЛЯ ОЧИСТКИ"};
inline constexpr LocalizedString SpaceFreed{L"SPACE FREED", L"ЗВІЛЬНЕНО МІСЦЯ", L"ОСВОБОЖДЕНО МЕСТА"};
inline constexpr LocalizedString CleanupResult{L"CLEANUP RESULT", L"РЕЗУЛЬТАТ ОЧИЩЕННЯ", L"РЕЗУЛЬТАТ ОЧИСТКИ"};
inline constexpr LocalizedString NoErrors{L"No real errors", L"Реальних помилок немає", L"Реальных ошибок нет"};
inline constexpr LocalizedString CleanUp{L"Clean Up", L"Очистити", L"Очистить"};
inline constexpr LocalizedString NothingSelected{L"Nothing selected", L"Нічого не вибрано", L"Ничего не выбрано"};
inline constexpr LocalizedString IncludesSystemAction{L"includes system action", L"містить системну дію", L"содержит системное действие"};
inline constexpr LocalizedString RecommendedApplied{L"Recommended preset applied.", L"Рекомендований набір застосовано.", L"Рекомендуемый набор применён."};
inline constexpr LocalizedString AllSafeSelected{L"All safe categories selected.", L"Вибрано всі безпечні категорії.", L"Выбраны все безопасные категории."};
inline constexpr LocalizedString SelectionCleared{L"Selection cleared.", L"Вибір очищено.", L"Выбор очищен."};
inline constexpr LocalizedString Stopping{L"Stopping…", L"Зупинка…", L"Остановка…"};
inline constexpr LocalizedString Stop{L"Stop", L"Зупинити", L"Остановить"};
inline constexpr LocalizedString ScanAgain{L"Scan again", L"Сканувати знову", L"Сканировать снова"};
inline constexpr LocalizedString Scan{L"Scan", L"Сканувати", L"Сканировать"};
inline constexpr LocalizedString Subtitle{L"Safe cleanup with clear controls and transparent results", L"Безпечне очищення зі зрозумілим керуванням і прозорими результатами", L"Безопасная очистка с понятным управлением и прозрачными результатами"};
inline constexpr LocalizedString WindowsStorage{L"Windows Storage  ↗", L"Сховище Windows  ↗", L"Хранилище Windows  ↗"};
inline constexpr LocalizedString Loading{L"Loading…", L"Завантаження…", L"Загрузка…"};
inline constexpr LocalizedString ScanNotRun{L"Scan has not been run yet", L"Сканування ще не запускалося", L"Сканирование ещё не запускалось"};
inline constexpr LocalizedString ItemsToClean{L"Items to Clean", L"Що очистити", L"Что очистить"};
inline constexpr LocalizedString ItemsHint{L"Scan first, then choose what to remove", L"Спочатку проскануйте, потім виберіть, що видалити", L"Сначала просканируйте, затем выберите, что удалить"};
inline constexpr LocalizedString Recommended{L"Recommended", L"Рекомендовано", L"Рекомендуется"};
inline constexpr LocalizedString AllSafe{L"All Safe", L"Усе безпечне", L"Всё безопасное"};
inline constexpr LocalizedString Reset{L"Reset", L"Скинути", L"Сбросить"};
inline constexpr LocalizedString ProtectRecent{L"Protect files modified in the last 24 hours", L"Захищати файли, змінені протягом останніх 24 годин", L"Защищать файлы, изменённые за последние 24 часа"};
inline constexpr LocalizedString Activity{L"Activity", L"Активність", L"Активность"};
inline constexpr LocalizedString ActivityHint{L"Detailed operation log", L"Докладний журнал операцій", L"Подробный журнал операций"};
inline constexpr LocalizedString ExportTxt{L"Export TXT", L"Експорт TXT", L"Экспорт TXT"};
inline constexpr LocalizedString Clear{L"Clear", L"Очистити", L"Очистить"};
inline constexpr LocalizedString ReadyToScan{L"Ready to scan", L"Готово до сканування", L"Готово к сканированию"};
inline constexpr LocalizedString RecommendedSelected{L"Recommended options are already selected", L"Рекомендовані параметри вже вибрано", L"Рекомендуемые параметры уже выбраны"};
}

namespace process {
inline constexpr LocalizedString GoogleChrome{L"Google Chrome", L"Google Chrome", L"Google Chrome"};
inline constexpr LocalizedString MicrosoftEdge{L"Microsoft Edge", L"Microsoft Edge", L"Microsoft Edge"};
inline constexpr LocalizedString Brave{L"Brave", L"Brave", L"Brave"};
inline constexpr LocalizedString Vivaldi{L"Vivaldi", L"Vivaldi", L"Vivaldi"};
inline constexpr LocalizedString Opera{L"Opera", L"Opera", L"Opera"};
inline constexpr LocalizedString Firefox{L"Firefox", L"Firefox", L"Firefox"};
inline constexpr LocalizedString Discord{L"Discord", L"Discord", L"Discord"};
inline constexpr LocalizedString Slack{L"Slack", L"Slack", L"Slack"};
inline constexpr LocalizedString Teams{L"Teams", L"Teams", L"Teams"};
inline constexpr LocalizedString NewTeams{L"New Teams", L"Новий Teams", L"Новый Teams"};
}

namespace report {
inline constexpr LocalizedString Title{L"N91 DISK CLEANER 2.0 — REPORT", L"N91 DISK CLEANER 2.0 — ЗВІТ", L"N91 DISK CLEANER 2.0 — ОТЧЁТ"};
inline constexpr LocalizedString SystemDrive{L"System drive: ", L"Системний диск: ", L"Системный диск: "};
inline constexpr LocalizedString FreeSpace{L"Free space: ", L"Вільне місце: ", L"Свободное место: "};
inline constexpr LocalizedString Of{L" of ", L" з ", L" из "};
inline constexpr LocalizedString LastCleanup{L"LAST CLEANUP", L"ОСТАННЄ ОЧИЩЕННЯ", L"ПОСЛЕДНЯЯ ОЧИСТКА"};
inline constexpr LocalizedString FilesDeleted{L"Files deleted: ", L"Видалено файлів: ", L"Удалено файлов: "};
inline constexpr LocalizedString SpaceFreed{L"Space freed: ", L"Звільнено місця: ", L"Освобождено места: "};
inline constexpr LocalizedString FilesSkipped{L"Files skipped: ", L"Пропущено файлів: ", L"Пропущено файлов: "};
inline constexpr LocalizedString Errors{L"Real errors: ", L"Реальні помилки: ", L"Реальные ошибки: "};
inline constexpr LocalizedString RecentProtection{L"Recent-file protection (<24 h): ", L"Захист нещодавніх файлів (<24 год): ", L"Защита недавних файлов (<24 ч): "};
inline constexpr LocalizedString Enabled{L"Enabled", L"Увімкнено", L"Включено"};
inline constexpr LocalizedString Disabled{L"Disabled", L"Вимкнено", L"Отключено"};
inline constexpr LocalizedString Categories{L"CATEGORIES", L"КАТЕГОРІЇ", L"КАТЕГОРИИ"};
inline constexpr LocalizedString ActivityLog{L"ACTIVITY LOG", L"ЖУРНАЛ АКТИВНОСТІ", L"ЖУРНАЛ АКТИВНОСТИ"};
inline constexpr LocalizedString FilenamePattern{L"N91DiskCleaner_%04u-%02u-%02u_%02u-%02u.txt", L"N91DiskCleaner_%04u-%02u-%02u_%02u-%02u.txt", L"N91DiskCleaner_%04u-%02u-%02u_%02u-%02u.txt"};
}

namespace dialog {
inline constexpr LocalizedString TextFilter{
    L"Text report (*.txt)\0*.txt\0All files (*.*)\0*.*\0\0",
    L"Текстовий звіт (*.txt)\0*.txt\0Усі файли (*.*)\0*.*\0\0",
    L"Текстовый отчёт (*.txt)\0*.txt\0Все файлы (*.*)\0*.*\0\0"
};
inline constexpr LocalizedString ReportExported{L"Report exported: ", L"Звіт експортовано: ", L"Отчёт экспортирован: "};
inline constexpr LocalizedString ReportSaved{L"The report was saved successfully.", L"Звіт успішно збережено.", L"Отчёт успешно сохранён."};
inline constexpr LocalizedString ExportComplete{L"Export Complete", L"Експорт завершено", L"Экспорт завершён"};
inline constexpr LocalizedString ReportSaveFailedLog{L"Error: failed to save the report.", L"Помилка: не вдалося зберегти звіт.", L"Ошибка: не удалось сохранить отчёт."};
inline constexpr LocalizedString ReportSaveFailed{L"The report file could not be written.", L"Не вдалося записати файл звіту.", L"Не удалось записать файл отчёта."};
inline constexpr LocalizedString Error{L"Error", L"Помилка", L"Ошибка"};
inline constexpr LocalizedString StorageOpenFailed{L"Error: failed to open Windows Storage settings.", L"Помилка: не вдалося відкрити налаштування сховища Windows.", L"Ошибка: не удалось открыть параметры хранилища Windows."};
inline constexpr LocalizedString RunScanFirst{L"Run a scan first to review the estimated size and cleanup risks.", L"Спочатку запустіть сканування, щоб переглянути оцінений розмір і ризики очищення.", L"Сначала запустите сканирование, чтобы увидеть оценочный размер и риски очистки."};
inline constexpr LocalizedString ScanRequired{L"Scan Required", L"Потрібне сканування", L"Требуется сканирование"};
inline constexpr LocalizedString SelectCategory{L"Select at least one cleanup category.", L"Виберіть хоча б одну категорію очищення.", L"Выберите хотя бы одну категорию очистки."};
inline constexpr LocalizedString NothingSelected{L"Nothing Selected", L"Нічого не вибрано", L"Ничего не выбрано"};
inline constexpr LocalizedString ConfirmCleanup{L"Confirm Cleanup", L"Підтвердження очищення", L"Подтверждение очистки"};
inline constexpr LocalizedString OperationRunningPrompt{L"An operation is still running. Stop it and close the application after the current step finishes safely?", L"Операція ще виконується. Зупинити її та закрити програму після безпечного завершення поточного кроку?", L"Операция ещё выполняется. Остановить её и закрыть программу после безопасного завершения текущего шага?"};
inline constexpr LocalizedString OperationInProgress{L"Operation in Progress", L"Операція виконується", L"Операция выполняется"};
}

namespace scan {
inline constexpr LocalizedString Status{L"Scanning system", L"Сканування системи", L"Сканирование системы"};
inline constexpr LocalizedString Detail{L"Checking known cleanup locations only", L"Перевіряються лише відомі місця для очищення", L"Проверяются только известные места для очистки"};
inline constexpr LocalizedString Started{L"Started a read-only scan. No files will be deleted.", L"Запущено сканування лише для читання. Жодні файли не буде видалено.", L"Запущено сканирование только для чтения. Файлы удаляться не будут."};
inline constexpr LocalizedString ActiveVerb{L"Scanning", L"Сканування", L"Сканирование"};
inline constexpr LocalizedString UnexpectedWorkerError{L"Error: unexpected error in the scan worker.", L"Помилка: неочікувана помилка в робочому потоці сканування.", L"Ошибка: неожиданная ошибка в рабочем потоке сканирования."};
inline constexpr LocalizedString StartFailed{L"Unable to start scan", L"Не вдалося запустити сканування", L"Не удалось запустить сканирование"};
inline constexpr LocalizedString ThreadCreateFailedDetail{L"Windows could not create the scan worker thread", L"Windows не вдалося створити робочий потік сканування", L"Windows не удалось создать рабочий поток сканирования"};
inline constexpr LocalizedString ThreadCreateFailedLog{L"Error: failed to start the scan worker thread.", L"Помилка: не вдалося запустити робочий потік сканування.", L"Ошибка: не удалось запустить рабочий поток сканирования."};
inline constexpr LocalizedString Stopped{L"Scan stopped", L"Сканування зупинено", L"Сканирование остановлено"};
inline constexpr LocalizedString StoppedDetail{L"Run it again to get complete results", L"Запустіть його знову, щоб отримати повні результати", L"Запустите его снова, чтобы получить полные результаты"};
inline constexpr LocalizedString StoppedLog{L"Scan stopped by the user.", L"Сканування зупинено користувачем.", L"Сканирование остановлено пользователем."};
inline constexpr LocalizedString Failed{L"Scan failed", L"Помилка сканування", L"Ошибка сканирования"};
inline constexpr LocalizedString FailedDetail{L"See the activity log for details", L"Подробиці дивіться в журналі активності", L"Подробности смотрите в журнале активности"};
inline constexpr LocalizedString Complete{L"Scan complete", L"Сканування завершено", L"Сканирование завершено"};
inline constexpr LocalizedString CompleteDetail{L"Review the categories and confirm cleanup", L"Перегляньте категорії та підтвердьте очищення", L"Просмотрите категории и подтвердите очистку"};
inline constexpr LocalizedString CompleteLog{L"Scan complete. Results are ready for review.", L"Сканування завершено. Результати готові до перегляду.", L"Сканирование завершено. Результаты готовы к просмотру."};
inline constexpr LocalizedString ProtectedOrSkipped{L"protected or skipped", L"захищено або пропущено", L"защищено или пропущено"};
}

namespace confirmation {
inline constexpr LocalizedString SelectedSuffix{L" selected.\n", L" вибрано.\n", L" выбрано.\n"};
inline constexpr LocalizedString EstimatePrefix{L"Estimated space to free: ", L"Орієнтовно буде звільнено: ", L"Ориентировочно будет освобождено: "};
inline constexpr LocalizedString MorePrefix{L"• …and ", L"• …і ще ", L"• …и ещё "};
inline constexpr LocalizedString MoreSuffix{L" more\n", L"\n", L"\n"};
inline constexpr LocalizedString Caution{L"\nCaution items may remove cached data or files from the Recycle Bin.", L"\nОбережно: деякі пункти можуть видалити кешовані дані або файли з Кошика.", L"\nОсторожно: некоторые пункты могут удалить кэшированные данные или файлы из Корзины."};
inline constexpr LocalizedString System{L"\nSystem actions may take a while and change Windows settings.", L"\nСистемні дії можуть тривати певний час і змінювати налаштування Windows.", L"\nСистемные действия могут занять некоторое время и изменить параметры Windows."};
inline constexpr LocalizedString RunningApps{L"\n\nThe following apps are still running, and their caches are selected for cleanup:\n", L"\n\nЦі програми все ще працюють, а їхні кеші вибрано для очищення:\n", L"\n\nЭти программы всё ещё работают, а их кэши выбраны для очистки:\n"};
inline constexpr LocalizedString CloseApps{L"Close them for a more complete cleanup, or continue—files in use will be skipped.", L"Закрийте їх для повнішого очищення або продовжуйте — файли, що використовуються, буде пропущено.", L"Закройте их для более полной очистки или продолжайте — используемые файлы будут пропущены."};
inline constexpr LocalizedString Continue{L"\n\nContinue with cleanup?", L"\n\nПродовжити очищення?", L"\n\nПродолжить очистку?"};
}

namespace cleanup {
inline constexpr LocalizedString InProgress{L"Cleanup in progress", L"Триває очищення", L"Идёт очистка"};
inline constexpr LocalizedString InProgressDetail{L"Files in use and protected files will be skipped", L"Файли, що використовуються, і захищені файли буде пропущено", L"Используемые и защищённые файлы будут пропущены"};
inline constexpr LocalizedString Started{L"=== CLEANUP STARTED ===", L"=== ОЧИЩЕННЯ РОЗПОЧАТО ===", L"=== ОЧИСТКА НАЧАТА ==="};
inline constexpr LocalizedString ActiveVerb{L"Cleaning", L"Очищення", L"Очистка"};
inline constexpr LocalizedString ErrorPrefix{L"Error: ", L"Помилка: ", L"Ошибка: "};
inline constexpr LocalizedString WarningPrefix{L"Warning: ", L"Попередження: ", L"Предупреждение: "};
inline constexpr LocalizedString DonePrefix{L"Done: ", L"Готово: ", L"Готово: "};
inline constexpr LocalizedString FreedSuffix{L" freed", L" звільнено", L" освобождено"};
inline constexpr LocalizedString SkipsPrefix{L"Skips [", L"Пропуски [", L"Пропуски ["};
inline constexpr LocalizedString StartFailed{L"Unable to start cleanup", L"Не вдалося запустити очищення", L"Не удалось запустить очистку"};
inline constexpr LocalizedString ThreadCreateFailedDetail{L"Windows could not create the cleanup worker thread", L"Windows не вдалося створити робочий потік очищення", L"Windows не удалось создать рабочий поток очистки"};
inline constexpr LocalizedString ThreadCreateFailedLog{L"Error: failed to start the cleanup worker thread.", L"Помилка: не вдалося запустити робочий потік очищення.", L"Ошибка: не удалось запустить рабочий поток очистки."};
inline constexpr LocalizedString Stopping{L"Stopping operation", L"Зупинка операції", L"Остановка операции"};
inline constexpr LocalizedString StoppingDetail{L"The current system step will finish safely", L"Поточний системний крок буде безпечно завершено", L"Текущий системный шаг будет безопасно завершён"};
inline constexpr LocalizedString StopRequested{L"Stop requested by user.", L"Користувач запросив зупинку.", L"Пользователь запросил остановку."};
inline constexpr LocalizedString LogCleared{L"Activity log cleared.", L"Журнал активності очищено.", L"Журнал активности очищен."};
inline constexpr LocalizedString RescanRequired{L"Rescan required", L"Потрібне повторне сканування", L"Требуется повторное сканирование"};
inline constexpr LocalizedString ProtectionChanged{L"The recent-file protection policy changed", L"Політику захисту нещодавніх файлів змінено", L"Политика защиты недавних файлов изменена"};
inline constexpr LocalizedString ProtectionEnabled{L"Recent-file protection enabled (24 hours).", L"Захист нещодавніх файлів увімкнено (24 години).", L"Защита недавних файлов включена (24 часа)."};
inline constexpr LocalizedString ProtectionDisabled{L"Warning: recent-file protection disabled.", L"Попередження: захист нещодавніх файлів вимкнено.", L"Предупреждение: защита недавних файлов отключена."};
inline constexpr LocalizedString Summary{L"=== CLEANUP SUMMARY ===", L"=== ПІДСУМОК ОЧИЩЕННЯ ===", L"=== ИТОГ ОЧИСТКИ ==="};
inline constexpr LocalizedString Stopped{L"Cleanup stopped", L"Очищення зупинено", L"Очистка остановлена"};
inline constexpr LocalizedString CompletedErrors{L"Cleanup completed with errors", L"Очищення завершено з помилками", L"Очистка завершена с ошибками"};
inline constexpr LocalizedString CompletedSkips{L"Cleanup completed with skipped files", L"Очищення завершено з пропущеними файлами", L"Очистка завершена с пропущенными файлами"};
inline constexpr LocalizedString CompletedSuccess{L"Cleanup completed successfully", L"Очищення успішно завершено", L"Очистка успешно завершена"};
}

inline int SlavicPluralForm(std::uint64_t count) noexcept {
    const std::uint64_t mod10 = count % 10;
    const std::uint64_t mod100 = count % 100;
    if (mod10 == 1 && mod100 != 11) return 0;
    if (mod10 >= 2 && mod10 <= 4 && (mod100 < 12 || mod100 > 14)) return 1;
    return 2;
}

inline std::wstring Count(std::uint64_t count, const wchar_t* singular, const wchar_t* plural) {
    return std::to_wstring(count) + L" " + (count == 1 ? singular : plural);
}

inline std::wstring Files(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"file" : L"files");
    const int form = SlavicPluralForm(count);
    if (GetLanguage() == Language::Ukrainian) {
        static constexpr const wchar_t* forms[] = {L"файл", L"файли", L"файлів"};
        return std::to_wstring(count) + L" " + forms[form];
    }
    static constexpr const wchar_t* forms[] = {L"файл", L"файла", L"файлов"};
    return std::to_wstring(count) + L" " + forms[form];
}

inline std::wstring Categories(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"category" : L"categories");
    const int form = SlavicPluralForm(count);
    if (GetLanguage() == Language::Ukrainian) {
        static constexpr const wchar_t* forms[] = {L"категорія", L"категорії", L"категорій"};
        return std::to_wstring(count) + L" " + forms[form];
    }
    static constexpr const wchar_t* forms[] = {L"категория", L"категории", L"категорий"};
    return std::to_wstring(count) + L" " + forms[form];
}

inline std::wstring Items(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"item" : L"items");
    const int form = SlavicPluralForm(count);
    if (GetLanguage() == Language::Ukrainian) {
        static constexpr const wchar_t* forms[] = {L"об’єкт", L"об’єкти", L"об’єктів"};
        return std::to_wstring(count) + L" " + forms[form];
    }
    static constexpr const wchar_t* forms[] = {L"объект", L"объекта", L"объектов"};
    return std::to_wstring(count) + L" " + forms[form];
}

inline std::wstring CleanupItems(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"cleanup item" : L"cleanup items");
    const int form = SlavicPluralForm(count);
    if (GetLanguage() == Language::Ukrainian) {
        static constexpr const wchar_t* forms[] = {L"об’єкт очищення", L"об’єкти очищення", L"об’єктів очищення"};
        return std::to_wstring(count) + L" " + forms[form];
    }
    static constexpr const wchar_t* forms[] = {L"объект очистки", L"объекта очистки", L"объектов очистки"};
    return std::to_wstring(count) + L" " + forms[form];
}

inline std::wstring FilesDeleted(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"file deleted" : L"files deleted");
    return GetLanguage() == Language::Ukrainian
        ? L"видалено " + Files(count)
        : L"удалено " + Files(count);
}

inline std::wstring FilesSkipped(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"file skipped" : L"files skipped");
    return GetLanguage() == Language::Ukrainian
        ? L"пропущено " + Files(count)
        : L"пропущено " + Files(count);
}

inline std::wstring Errors(std::uint64_t count) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + L" " + (count == 1 ? L"real error" : L"real errors");
    const int form = SlavicPluralForm(count);
    if (GetLanguage() == Language::Ukrainian) {
        static constexpr const wchar_t* forms[] = {L"реальна помилка", L"реальні помилки", L"реальних помилок"};
        return std::to_wstring(count) + L" " + forms[form];
    }
    static constexpr const wchar_t* forms[] = {L"реальная ошибка", L"реальные ошибки", L"реальных ошибок"};
    return std::to_wstring(count) + L" " + forms[form];
}

inline std::wstring HiddenMessages(std::uint64_t count, const std::wstring& issueLabel) {
    if (GetLanguage() == Language::English)
        return std::to_wstring(count) + (count == 1 ? L" more similar message hidden · " : L" more similar messages hidden · ") + issueLabel;
    if (GetLanguage() == Language::Ukrainian)
        return std::to_wstring(count) + (count == 1 ? L" схоже повідомлення приховано · " : L" схожих повідомлень приховано · ") + issueLabel;
    return std::to_wstring(count) + (count == 1 ? L" похожее сообщение скрыто · " : L" похожих сообщений скрыто · ") + issueLabel;
}

inline std::wstring DriveUsage(int percent, const std::wstring& total) {
    if (GetLanguage() == Language::Ukrainian)
        return std::to_wstring(percent) + L"% використано · усього " + total;
    if (GetLanguage() == Language::Russian)
        return std::to_wstring(percent) + L"% использовано · всего " + total;
    return std::to_wstring(percent) + L"% used · " + total + L" total";
}

inline std::wstring ScanSummary(std::uint64_t files, std::uint64_t categories) {
    return Files(files) + L" · " + Categories(categories);
}

inline std::wstring SelectionSummary(std::uint64_t items, bool hasSystemAction) {
    std::wstring result = items ? Items(items) : std::wstring(ui::NothingSelected.c_str());
    if (hasSystemAction) result += L" · " + std::wstring(ui::IncludesSystemAction.c_str());
    return result;
}

inline std::wstring ProgressStatus(const wchar_t* verb,
                                   std::uint64_t current,
                                   std::uint64_t total,
                                   const std::wstring& title) {
    const wchar_t* connector = L" of ";
    if (GetLanguage() == Language::Ukrainian) connector = L" з ";
    else if (GetLanguage() == Language::Russian) connector = L" из ";
    return std::wstring(verb) + L" " + std::to_wstring(current) + connector +
           std::to_wstring(total) + L" · " + title;
}

inline std::wstring CategoryCleanupResult(bool hasErrors,
                                          const std::wstring& title,
                                          std::uint64_t deletedFiles,
                                          const std::wstring& freed,
                                          std::uint64_t skippedFiles,
                                          std::uint64_t errors) {
    const wchar_t* prefix = hasErrors ? cleanup::ErrorPrefix.c_str() : cleanup::DonePrefix.c_str();
    return std::wstring(prefix) + title + L" · " + FilesDeleted(deletedFiles) + L" · " +
           freed + cleanup::FreedSuffix.c_str() + L" · " + FilesSkipped(skippedFiles) + L" · " + Errors(errors);
}

inline std::wstring CleanupMetrics(std::uint64_t deletedFiles,
                                   const std::wstring& freed,
                                   std::uint64_t skippedFiles,
                                   std::uint64_t errors) {
    return FilesDeleted(deletedFiles) + L" · " + freed + cleanup::FreedSuffix.c_str() + L" · " +
           FilesSkipped(skippedFiles) + L" · " + Errors(errors);
}

inline std::wstring ScanItemResult(const std::wstring& title,
                                   const std::wstring& size,
                                   std::uint64_t files,
                                   std::uint64_t skipped) {
    std::wstring result = title + L": " + size + L" · " + Files(files);
    if (skipped) result += L" · " + FilesSkipped(skipped);
    return result;
}

} // namespace english

} // namespace localization
