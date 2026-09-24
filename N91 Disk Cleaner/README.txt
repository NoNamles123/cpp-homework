N91 DISK CLEANER — C++ / WIN32
===============================

N91 Disk Cleaner is a native C++ application for Windows 10 and 11.
After compilation, it runs as a regular executable without BAT, VBS, or PowerShell windows.
BUILD_RELEASE.cmd or build_release.bat is needed only to compile the application, not to run it.

VERSION 2.0 FEATURES
--------------------
- Modern dark interface, responsive window sizing, and per-monitor DPI support.
- Runtime interface language selection: English, Ukrainian, and Russian.
- Automatic initial language detection based on the Windows display language.
- Dashboard with drive capacity, scan results, selected size, and file counts.
- Safe read-only scan before any deletion.
- Recommended, All Safe, and Reset presets.
- Protection for files modified within the last 24 hours.
- Clear Safe, Caution, and System risk labels.
- Color-coded activity log, progress tracking, and safe cancellation.
- Separate statistics for files deleted, space freed, files skipped, and errors.
- Busy, protected, unavailable, and already missing files are skipped instead of counted as real errors.
- Symbolic links, reparse-backed entries, and mounted offline Windows images are protected.
- Repeated errors are grouped by category and type, with no more than 20 examples in the log.
- Detection of running browsers and messaging apps before cache cleanup.
- Full TXT report export with Ctrl+S.
- Direct access to Windows Storage settings.
- Keyboard navigation, F5 to scan, and Esc to stop.
- Automatic administrator elevation through the application manifest.

LANGUAGES
---------
- English, Ukrainian, and Russian interface languages are included in Localization.h.
- On startup, the application follows the Windows display language.
- Unsupported system languages fall back to English.
- The language can be changed instantly from the selector in the application header.
- The selected language is session-only; the Windows language is detected again after restart.
- Existing activity-log lines keep the language in which they were created. New lines use the newly selected language.

CLEANUP CATEGORIES
------------------
- User Temp, Windows Temp, and Windows Error Reporting.
- Delivery Optimization and Windows Update downloads.
- Windows thumbnail and icon caches.
- Chrome, Edge, Brave, Vivaldi, Opera GX, and Firefox caches.
- Discord, Slack, and Microsoft Teams caches.
- DirectX, NVIDIA, and AMD shader caches.
- Crash dumps, the Recycle Bin, and DISM component cleanup.
- Optional action to disable hibernation.

NEVER REMOVED
-------------
- Downloads, Desktop, Documents, Pictures, or Videos.
- Games or installed applications.
- The page file.
- Restore points.
- Drivers.
- Windows Installer or Package Cache data.

BUILD WITH VISUAL STUDIO
------------------------
1. Install Visual Studio Community or Build Tools with a compatible MSVC toolset.
2. Add the "Desktop development with C++" workload.
3. Open N91DiskCleaner.sln.
4. Select Release and x64.
5. Choose Build > Build Solution.
6. The finished executable will be at x64\Release\N91DiskCleaner.exe.

BUILD WITH CMAKE
----------------
Run BUILD_RELEASE.cmd. The script automatically finds CMake, including the copy bundled with Visual Studio.

The finished executable will be at:
build-n91\Release\N91DiskCleaner.exe

RUN REGRESSION TESTS
--------------------
cmake -S . -B build-n91 -A x64 -DBUILD_TESTING=ON
cmake --build build-n91 --config Release
ctest --test-dir build-n91 -C Release --output-on-failure

The tests verify cleanup error classification and grouping, the 20-message log limit,
24-hour file protection, offline Windows-image protection, safe test-file deletion,
runtime language selection, translated labels, and English/Ukrainian/Russian plural forms.

LICENSE
-------
The source code is provided under the N91 Disk Cleaner Source Code License.
See LICENSE.txt before using, modifying, or sharing the source code.

IMPORTANT
---------
- Close browsers, messaging apps, and games before cleaning related caches.
- The Recommended preset does not select the Recycle Bin, DISM, Windows Update, or hibernation.
- With 24-hour protection enabled, recently modified and active files are not deleted.
- The first game launch after shader cache cleanup may briefly stutter while shaders rebuild.
- Disabling hibernation also disables Windows Fast Startup.
- Windows SmartScreen may display a warning because the executable is not digitally signed by a trusted publisher.
