#include <bits/stdc++.h>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// ==================== COLOR DEFINITIONS ====================
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// Background colors
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"

// Style
#define BOLD "\033[1m"
#define RESET "\033[0m"

using namespace std;

const string LOGIN_FILE = "login.txt";
const double DAILY_MEAL_RATE = 65.0;

void clearScreen()
{
    system("clear");
}

void pauseAndClear()
{
    cout << MAGENTA << BOLD << "\nPress any key to continue" << RESET;

    const char *dots[] = {".", "..", "..."};
    int i = 0;

    while (true)
    {
        cout << "\r" << MAGENTA << BOLD << "Press any key to continue" << RESET
             << YELLOW << dots[i % 3] << RESET << "   ";
        cout.flush();

        i++;

        // For Mac - check if input is available
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 500000; // 0.5 second

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        int ret = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

        if (ret > 0)
        {
            system("afplay /System/Library/Sounds/Tink.aiff &");
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    cout << "\n";
    clearScreen();
}

// ==================== UI HELPER FUNCTIONS ====================

void printHeader(const string &title)
{
    cout << CYAN << BOLD;
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

    int totalWidth = 64;
    int padding = (totalWidth - 2 - title.length()) / 2;

    cout << "║" << string(padding, ' ') << title
         << string(totalWidth - 2 - padding - title.length(), ' ') << "  ║\n";

    cout << "╚════════════════════════════════════════════════════════════════╝\n"
         << RESET;
}

void printSuccess(const string &message)
{
    cout << GREEN << "\n✓ " << message << RESET << "\n";
}

void printError(const string &message)
{
    cout << RED << "\n✗ " << message << RESET << "\n";
}

void printWarning(const string &message)
{
    cout << YELLOW << "\n⚠ " << message << RESET << "\n";
}

void printInfo(const string &message)
{
    cout << CYAN << "\nℹ " << message << RESET << "\n";
}

void printSeparator()
{
    cout << BLUE << "═════════════════════════════════════════════════════════════════════════\n"
         << RESET;
}

void printMenuOption(int number, const string &option, bool highlight = false)
{
    if (highlight)
    {
        cout << YELLOW << " → " << number << ". " << option << RESET << "\n";
    }
    else
    {
        cout << CYAN << "   " << number << ". " << option << RESET << "\n";
    }
}

void printBox(const string &content)
{
    int length = content.length();
    // Use ASCII fallback for box drawing to avoid character encoding issues
    cout << BLUE << "+" << string(length + 2, '-') << "+\n";
    cout << "| " << WHITE << content << BLUE << " |\n";
    cout << "+" << string(length + 2, '-') << "+" << RESET << "\n";
}

void printTableHeader(const vector<string> &headers, const vector<int> &widths)
{
    cout << CYAN << BOLD;
    for (size_t i = 0; i < headers.size(); i++)
    {
        cout << left << setw(widths[i]) << headers[i];
    }
    cout << RESET << "\n";

    int totalWidth = 0;
    for (int w : widths)
        totalWidth += w;
    cout << BLUE << string(totalWidth, '-') << RESET << "\n";
}

void printTableRow(const vector<string> &data, const vector<int> &widths, const string &color = "")
{
    for (size_t i = 0; i < data.size(); i++)
    {
        if (color == "green")
            cout << GREEN;
        else if (color == "red")
            cout << RED;
        else if (color == "yellow")
            cout << YELLOW;

        cout << left << setw(widths[i]) << data[i];

        if (!color.empty())
            cout << RESET;
    }
    cout << "\n";
}

void showLoadingAnimation(const string &message, int duration = 2)
{
    const char animation[] = {'|', '/', '-', '\\'};

    for (int i = 0; i < duration * 10; i++)
    {
        cout << CYAN << "\r" << message << " " << animation[i % 4] << RESET;
        cout.flush();
        usleep(100000);
    }
    cout << "\r" << string(message.length() + 10, ' ') << "\r";
}

void printWelcomeBanner()
{
    clearScreen();
    cout << CYAN << BOLD;
    cout << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║        🏠  HALL MANAGEMENT SYSTEM  🏠                         ║
    ║                                                               ║
    ║               Your Home Away From Home                        ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
    )" << RESET
         << "\n\n";
}

void printGoodbye()
{
    clearScreen();
    cout << GREEN << BOLD;
    cout << R"(
    ╔═══════════════════════════════════════════════════════════════╗
    ║                                                               ║
    ║                  Thank You for Using                          ║
    ║            🏠  HALL MANAGEMENT SYSTEM  🏠                     ║
    ║                                                               ║
    ║                    See You Soon! 👋                           ║
    ║                                                               ║
    ╚═══════════════════════════════════════════════════════════════╝
    )" << RESET
         << "\n\n";
}

void printDashboardHeader(const string &role, const string &name)
{
    clearScreen();
    cout << MAGENTA << BOLD;
    cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";

    string title = role + " DASHBOARD";
    int titlePadding = (63 - title.length()) / 2;
    cout << "║" << string(titlePadding, ' ') << title
         << string(63 - titlePadding - title.length(), ' ') << "║\n";

    cout << "╠═══════════════════════════════════════════════════════════════╣\n";

    string welcome = "Welcome, " + name;
    int welcomePadding = (63 - welcome.length()) / 2;
    cout << "║" << string(welcomePadding, ' ') << CYAN << welcome << MAGENTA
         << string(63 - welcomePadding - welcome.length(), ' ') << "║\n";

    cout << "╚═══════════════════════════════════════════════════════════════╝\n"
         << RESET;
}

void printPaymentSlip(const string &paymentId, const string &studentName,
                      const string &month, double amount, const string &status)
{
    clearScreen();
    cout << CYAN << BOLD;
    cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    cout << "║                     💰 PAYMENT SLIP 💰                        ║\n";
    cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    cout << RESET;

    cout << CYAN << "║  " << RESET << "Payment ID    : " << YELLOW << paymentId << string(41 - paymentId.length(), ' ') << CYAN << "║\n";
    cout << "║  " << RESET << "Student Name  : " << WHITE << studentName << string(41 - studentName.length(), ' ') << CYAN << "║\n";
    cout << "║  " << RESET << "Month         : " << WHITE << month << string(41 - month.length(), ' ') << CYAN << "║\n";
    cout << "║  " << RESET << "Amount        : " << GREEN << fixed << setprecision(2) << amount << " Tk"
         << string(37 - to_string((int)amount).length(), ' ') << CYAN << "║\n";

    if (status == "paid")
        cout << "║  " << RESET << "Status        : " << GREEN << "✓ PAID" << string(41, ' ') << CYAN << "║\n";
    else
        cout << "║  " << RESET << "Status        : " << RED << "✗ PENDING" << string(37, ' ') << CYAN << "║\n";

    cout << "╚═══════════════════════════════════════════════════════════════╝\n"
         << RESET;
}

// ==================== ENHANCED MENU DISPLAYS ====================

void displayMainMenu()
{
    printWelcomeBanner();

    cout << CYAN << BOLD << "   MAIN MENU\n"
         << RESET;
    printSeparator();

    printMenuOption(1, "🔐 Administration Login");
    printMenuOption(2, "📝 Administration Registration");
    printMenuOption(3, "👤 Student Login");
    printMenuOption(4, "🔑 Forgot Password");
    printMenuOption(5, "🚪 Exit", true);

    printSeparator();
    cout << YELLOW << "\n   Enter your choice: " << RESET;
}

void displayAdministrationMenu(const string &administrationName)
{
    printDashboardHeader("ADMINISTRATION", administrationName);

    cout << "\n"
         << CYAN << BOLD << "   📊 MAIN OPERATIONS\n"
         << RESET;
    printMenuOption(1, "👥 Add Student");
    printMenuOption(2, "📋 View Students");
    printMenuOption(3, "🗑️  Delete Student");

    cout << "\n"
         << CYAN << BOLD << "   🏠 ROOM MANAGEMENT\n"
         << RESET;
    printMenuOption(4, "➕ Add Room");
    printMenuOption(5, "🚪 View Rooms");
    printMenuOption(6, "🔀 Assign Room");

    cout << "\n"
         << CYAN << BOLD << "   📢 COMPLAINTS & BILLING\n"
         << RESET;
    printMenuOption(7, "📝 View Complaints");
    printMenuOption(8, "💬 Reply to Complaint");
    printMenuOption(9, "🍽️  Update Daily Menu");
    printMenuOption(10, "💰 Calculate & Generate Bill");
    printMenuOption(11, "📊 View Payment Records");
    printMenuOption(12, "🔍 Search Student Payment");

    cout << "\n"
         << CYAN << BOLD << "   ⚙️  SYSTEM\n"
         << RESET;
    printMenuOption(13, "🗑️  Delete Administration Account");
    printMenuOption(14, "🚪 Logout");
    printMenuOption(15, "💾 Save & Exit", true);

    cout << "\n"
         << CYAN << BOLD << "   📊 ANALYTICS\n"
         << RESET;
    printMenuOption(16, "View Dashboard Analytics");

    printSeparator();
    cout << YELLOW << "\n   Enter your choice: " << RESET;
}

void displayStudentMenu(const string &studentName)
{
    printDashboardHeader("STUDENT", studentName);

    cout << "\n"
         << CYAN << BOLD << "   👤 PROFILE\n"
         << RESET;
    printMenuOption(1, "📋 View Profile");
    printMenuOption(2, "✏️  Edit Profile");
    printMenuOption(3, "🏠 View Room Info");

    cout << "\n"
         << CYAN << BOLD << "   🔄 REQUESTS\n"
         << RESET;
    printMenuOption(4, "📢 Complaint");
    printMenuOption(5, "🔀 Room Swap Request");
    printMenuOption(6, "👁️  View Swap Requests");

    cout << "\n"
         << CYAN << BOLD << "   🍽️  MEAL MANAGEMENT\n"
         << RESET;
    printMenuOption(7, "📝 Meal Management");
    printMenuOption(8, "📅 View Today's Menu");
    printMenuOption(9, "❌ Mark Meal Off");
    printMenuOption(10, "📊 View My Meal Plan");

    cout << "\n"
         << CYAN << BOLD << "   💰 PAYMENTS\n"
         << RESET;
    printMenuOption(11, "🧾 View Payment Slips");
    printMenuOption(12, "💳 Make Payment");

    cout << "\n"
         << CYAN << BOLD << "   ⚙️  SETTINGS\n"
         << RESET;
    printMenuOption(13, "🔑 Change Password");
    printMenuOption(14, "🚪 Log Out", true);

    printSeparator();
    cout << YELLOW << "\n   Enter your choice: " << RESET;
}

string readPassword()
{
    string password;
    char ch;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true)
    {
        ch = getchar();
        if (ch == '\n' || ch == '\r')
            break;
        else if (ch == 127 || ch == 8)
        {
            if (!password.empty())
            {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += ch;
            cout << '*';
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    cout << endl;
    return password;
}

string getCurrentDate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    snprintf(buffer, 20, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
    return string(buffer);
}

string getCurrentMonthYear()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[20];
    snprintf(buffer, 20, "%04d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon);
    return string(buffer);
}

int getDaysInMonth(const string &monthYear)
{
    int year, month;
    sscanf(monthYear.c_str(), "%d-%d", &year, &month);

    if (month == 2)
    {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        return 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    return 31;
}

class Student; // Forward declaration

class Person
{
protected:
    string name;
    string id;

public:
    Person(string name_ = "", string id_ = "") : name(name_), id(id_) {}
    virtual void display() const = 0;
    string getId() const { return id; }
    string getName() const { return name; }
    void setName(const string &newName) { name = newName; }
    virtual ~Person() {}
};

class Registration
{
private:
    string filename;

public:
    Registration(const string &file) : filename(file) {}

    bool fileExists()
    {
        ifstream fin(filename);
        return fin.good();
    }

    void registerAdministration();
};

class Login
{
private:
    string role, name, id, password, email, secQues, secAns;

public:
    Login(string r, string n, string i, string p, string e = "", string sq = "", string sa = "")
        : role(r), name(n), id(i), password(p), email(e), secQues(sq), secAns(sa) {}
    string getRole() const { return role; }
    string getName() const { return name; }
    string getId() const { return id; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
    string getSecQues() const { return secQues; }
    string getSecAns() const { return secAns; }
};

class LoginManager
{
private:
    string filename;

    void updatePasswordInFile(const string &role, const string &id, const string &newPass)
    {
        vector<string> lines;
        ifstream fin(filename);
        string line;

        while (getline(fin, line))
        {
            if (!line.empty())
            {
                stringstream ss(line);
                string r, n, i, p, e, sq, sa;
                getline(ss, r, '|');
                getline(ss, n, '|');
                getline(ss, i, '|');
                getline(ss, p, '|');
                getline(ss, e, '|');
                getline(ss, sq, '|');
                getline(ss, sa, '|');

                if (r == role && i == id)
                    line = r + "|" + n + "|" + i + "|" + newPass + "|" + e + "|" + sq + "|" + sa;
            }
            lines.push_back(line);
        }
        fin.close();

        ofstream fout(filename);
        for (const auto &l : lines)
            fout << l << "\n";
        fout.close();

        loadLogins();
    }

public:
    LoginManager(const string &file) : filename(file) {}
    vector<Login> logins;

    void loadLogins()
    {
        logins.clear();
        ifstream fin(filename);
        if (!fin.is_open())
            return;

        string line;
        while (getline(fin, line))
        {
            if (line.empty())
                continue;
            stringstream ss(line);
            string role, name, id, pass, email, secQues, secAns;
            getline(ss, role, '|');
            getline(ss, name, '|');
            getline(ss, id, '|');
            getline(ss, pass, '|');
            getline(ss, email, '|');
            getline(ss, secQues, '|');
            getline(ss, secAns, '|');
            if (!role.empty() && !id.empty() && !pass.empty())
                logins.push_back(Login(role, name, id, pass, email, secQues, secAns));
        }
        fin.close();
    }

    bool administrationLogin();

    string studentLogin();
    bool forgotPassword()
    {
        while (true)
        {
            clearScreen();

            // Beautiful header design
            cout << MAGENTA << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

            string title = "🔑 FORGOT PASSWORD";
            int titlePadding = (64 - title.length()) / 2;
            cout << "║" << string(titlePadding, ' ') << title
                 << string(64 - titlePadding - title.length(), ' ') << "  ║\n";

            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            // Helpful information
            cout << "\n"
                 << CYAN << BOLD << "  ℹ️  Password Recovery Options:\n"
                 << RESET;
            cout << "  " << YELLOW << "Choose your account type and recovery method to reset your password.\n"
                 << RESET;

            printSeparator();

            // Menu options with beautiful icons and descriptions
            cout << "\n"
                 << CYAN << BOLD << "  👨‍💼 Administration Account Recovery:\n"
                 << RESET;

            printMenuOption(1, "🛡️  Administration Password Reset", false);

            cout << "\n"
                 << CYAN << BOLD << "  👨‍🎓 Student Account Recovery:\n"
                 << RESET;

            printMenuOption(2, "🎓 Student Password Reset", false);

            cout << "\n"
                 << CYAN << BOLD << "  🔙 Navigation:\n"
                 << RESET;
            printMenuOption(3, "← Back to Main Menu", true);

            printSeparator();

            // Recovery methods information
            cout << "\n"
                 << YELLOW << BOLD << "  📧 Recovery Methods Available:\n"
                 << RESET;
            cout << "  ├─ " << GREEN << "✓ Email Verification" << RESET << "\n";
            cout << "  └─ " << BLUE << "✓ Security Question" << RESET << "\n";

            printSeparator();

            // Input section with validation
            cout << YELLOW << "\n   🎮 Enter your choice (1-3): " << RESET;

            int choice;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Show error with style
                clearScreen();
                cout << RED << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                      ❌ INVALID INPUT                          ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;
                cout << "\n"
                     << YELLOW << "   Please enter a number between 1 and 3." << RESET << "\n";

                // Wait and continue
                cout << "\n"
                     << CYAN << "   Press Enter to continue..." << RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                continue;
            }

            if (choice == 3)
            {
                clearScreen();
                printSuccess("Returning to Main Menu...");
                pauseAndClear();
                return false;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string role;
            string userType;

            if (choice == 1)
            {
                role = "administration";
                userType = "Administration";
            }
            else if (choice == 2)
            {
                role = "student";
                userType = "Student";
            }
            else
            {
                // Handle invalid choice (though it should be validated earlier)
                role = "unknown";
                userType = "Unknown";
                printError("Invalid user type selection!");
                pauseAndClear();
                return false;
            }

            // Show loading animation
            showLoadingAnimation("Loading " + userType + " Password Reset", 1);

            clearScreen();

            // Password reset header
            cout << MAGENTA << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

            string resetTitle = "🔑 " + userType + " PASSWORD RESET";
            int resetPadding = (64 - resetTitle.length()) / 2;
            cout << "║" << string(resetPadding, ' ') << resetTitle
                 << string(64 - resetPadding - resetTitle.length(), ' ') << "  ║\n";

            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            cout << YELLOW << "\n  Enter your " << userType << " ID: " << RESET;
            string id;
            getline(cin, id);

            // Search for user
            bool userFound = false;
            Login *targetUser = nullptr;

            for (Login &l : logins)
            {
                if (l.getRole() == role && l.getId() == id)
                {
                    userFound = true;
                    targetUser = &l;
                    break;
                }
            }

            if (!userFound)
            {
                clearScreen();
                cout << RED << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                     ❌ USER NOT FOUND                          ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;
                cout << "\n"
                     << YELLOW << "   " << userType << " ID '" << id << "' not found in the system." << RESET << "\n";
                pauseAndClear();
                continue;
            }

            // Recovery methods
            clearScreen();
            cout << CYAN << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                   CHOOSE RECOVERY METHOD                    ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            cout << "\n"
                 << YELLOW << "  Account: " << RESET << targetUser->getName() << " (" << targetUser->getId() << ")\n\n";

            cout << CYAN << BOLD << "  Available Recovery Methods:\n"
                 << RESET;

            bool emailAvailable = !targetUser->getEmail().empty();
            bool securityQAvailable = !targetUser->getSecQues().empty();

            if (emailAvailable)
            {
                printMenuOption(1, "📧 Email Verification", false);
                cout << "     " << WHITE << "Verify using registered email" << RESET << "\n";
            }
            else
            {
                cout << RED << "   ✗ Email Verification - Not available (No email registered)" << RESET << "\n";
            }

            if (securityQAvailable)
            {
                printMenuOption(2, "🔐 Security Question", false);
                cout << "     " << WHITE << "Answer your security question" << RESET << "\n";
            }
            else
            {
                cout << RED << "   ✗ Security Question - Not available (No question set)" << RESET << "\n";
            }

            printMenuOption(3, "↩️  Go Back", true);

            printSeparator();

            if (!emailAvailable && !securityQAvailable)
            {
                cout << "\n"
                     << RED << BOLD << "  ⚠️  NO RECOVERY METHODS AVAILABLE\n"
                     << RESET;
                cout << YELLOW << "  Please contact system administration for password reset.\n"
                     << RESET;
                cout << "\n"
                     << CYAN << "   Press Enter to continue..." << RESET;
                cin.get();
                continue;
            }

            cout << YELLOW << "\n  Choose recovery method (1-3): " << RESET;
            int method;
            if (!(cin >> method))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                printError("Invalid input!");
                pauseAndClear();
                continue;
            }

            if (method == 3)
            {
                continue; // Go back to previous menu
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            bool success = false;

            if (method == 1 && emailAvailable)
            {
                string email;
                cout << YELLOW << "\n  Enter your registered email: " << RESET;
                getline(cin, email);

                if (email == targetUser->getEmail())
                {
                    string newPass;
                    cout << YELLOW << "\n  ✅ Email verified! Enter new password: " << RESET;
                    newPass = readPassword();

                    if (newPass.empty())
                    {
                        printError("Password cannot be empty!");
                        pauseAndClear();
                        continue;
                    }

                    updatePasswordInFile(role, id, newPass);
                    printSuccess("Password reset successful!");
                    success = true;
                }
                else
                {
                    printError("Email does not match!");
                }
            }
            else if (method == 2 && securityQAvailable)
            {
                cout << "\n"
                     << CYAN << "  Security Question: " << YELLOW << targetUser->getSecQues() << RESET << "\n";
                cout << YELLOW << "  Your Answer: " << RESET;
                string ans;
                getline(cin, ans);

                if (ans == targetUser->getSecAns())
                {
                    string newPass;
                    cout << YELLOW << "\n  ✅ Answer verified! Enter new password: " << RESET;
                    newPass = readPassword();

                    if (newPass.empty())
                    {
                        printError("Password cannot be empty!");
                        pauseAndClear();
                        continue;
                    }

                    updatePasswordInFile(role, id, newPass);
                    printSuccess("Password reset successful!");
                    success = true;
                }
                else
                {
                    printError("Incorrect answer!");
                }
            }
            else
            {
                printError("Invalid choice or method not available!");
            }

            if (success)
            {
                printBox("You can now login with your new password! 🔑");
            }

            pauseAndClear();
            return success;
        }
    }
};

class MealPlan
{
public:
    string studentId;
    bool hasLunch;
    bool hasDinner;
    set<string> offDates;

    MealPlan(string sid = "", bool lunch = true, bool dinner = true)
        : studentId(sid), hasLunch(lunch), hasDinner(dinner) {}

    // OPERATOR OVERLOADING
    bool operator==(const MealPlan &other) const
    {
        return studentId == other.studentId;
    }

    bool operator==(const string &searchId) const
    {
        return studentId == searchId;
    }

    // Bool conversion for active check
    operator bool() const
    {
        return hasLunch || hasDinner;
    }

    // Combined meal check
    bool operator&&(const MealPlan &other) const
    {
        return (hasLunch && other.hasLunch) && (hasDinner && other.hasDinner);
    }

    // Output operator
    friend ostream &operator<<(ostream &os, const MealPlan &mp)
    {
        os << "MealPlan[Student:" << mp.studentId << ", Lunch:" << mp.hasLunch << ", Dinner:" << mp.hasDinner << "]";
        return os;
    }

    string serialize() const
    {
        string offDatesStr = "-";
        if (!offDates.empty())
        {
            auto it = offDates.begin();
            offDatesStr = *it;
            ++it;
            for (; it != offDates.end(); ++it)
                offDatesStr += "," + *it;
        }
        return studentId + "|" + (hasLunch ? "1" : "0") + "|" + (hasDinner ? "1" : "0") + "|" + offDatesStr;
    }

    static MealPlan deserialize(const string &line)
    {
        vector<string> parts;
        stringstream ss(line);
        string temp;
        while (getline(ss, temp, '|'))
            parts.push_back(temp);

        MealPlan mp;
        if (parts.size() > 0)
            mp.studentId = parts[0];
        if (parts.size() > 1)
            mp.hasLunch = (parts[1] == "1");
        if (parts.size() > 2)
            mp.hasDinner = (parts[2] == "1");
        if (parts.size() > 3 && parts[3] != "-")
        {
            stringstream offSS(parts[3]);
            string date;
            while (getline(offSS, date, ','))
                mp.offDates.insert(date);
        }
        return mp;
    }

    void display() const
    {
        cout << MAGENTA << "  🍽️  MealPlan[" << RESET << studentId << MAGENTA << "] " << RESET;
        cout << "Lunch: " << (hasLunch ? GREEN "✓" : RED "✗") << RESET;
        cout << " | Dinner: " << (hasDinner ? GREEN "✓" : RED "✗") << RESET;
        cout << " | Off Days: " << YELLOW << offDates.size() << RESET << "\n";
    }
};

class DailyMenu
{
public:
    string date;
    string lunchMenu;
    string dinnerMenu;

    DailyMenu(string d = "", string l = "", string dn = "")
        : date(d), lunchMenu(l), dinnerMenu(dn) {}

    // OPERATOR OVERLOADING
    bool operator==(const DailyMenu &other) const
    {
        return date == other.date;
    }

    bool operator==(const string &dateStr) const
    {
        return date == dateStr;
    }

    bool operator<(const DailyMenu &other) const
    {
        return date < other.date;
    }

    // Check if menu is set
    operator bool() const
    {
        return !lunchMenu.empty() || !dinnerMenu.empty();
    }

    // Output operator
    friend ostream &operator<<(ostream &os, const DailyMenu &dm)
    {
        os << "DailyMenu[Date:" << dm.date << ", Lunch:" << (dm.lunchMenu.empty() ? "Not set" : "Set") << "]";
        return os;
    }

    string serialize() const
    {
        return date + "|" + lunchMenu + "|" + dinnerMenu;
    }

    static DailyMenu deserialize(const string &line)
    {
        vector<string> parts;
        stringstream ss(line);
        string temp;
        while (getline(ss, temp, '|'))
            parts.push_back(temp);

        string d = parts.size() > 0 ? parts[0] : "";
        string l = parts.size() > 1 ? parts[1] : "";
        string dn = parts.size() > 2 ? parts[2] : "";
        return DailyMenu(d, l, dn);
    }

    void display() const
    {
        cout << GREEN << "  📋 Menu[" << RESET << date << GREEN << "] " << RESET;
        cout << "Lunch: " << (lunchMenu.empty() ? RED "Not set" : WHITE + lunchMenu) << RESET;
        cout << " | Dinner: " << (dinnerMenu.empty() ? RED "Not set" : WHITE + dinnerMenu) << RESET << "\n";
    }
};

class Payment
{
public:
    string paymentId;
    string studentId;
    string monthYear;
    double amount;
    string status;
    string date;

    Payment(string pid = "", string sid = "", string my = "", double amt = 0, string st = "pending", string d = "")
        : paymentId(pid), studentId(sid), monthYear(my), amount(amt), status(st), date(d) {}

    // OPERATOR OVERLOADING - FIXED
    bool operator==(const Payment &other) const
    {
        return paymentId == other.paymentId;
    }

    // Compare with payment ID
    bool operator==(const string &searchPaymentId) const
    {
        return paymentId == searchPaymentId;
    }

    // Compare with student ID - use different function name or parameter
    bool hasStudentId(const string &searchStudentId) const
    {
        return studentId == searchStudentId;
    }

    // Compare payment status
    bool hasStatus(const string &statusStr) const
    {
        return status == statusStr;
    }

    bool operator<(const Payment &other) const
    {
        return amount < other.amount;
    }

    bool operator>(const Payment &other) const
    {
        return amount > other.amount;
    }

    // Output operator
    friend ostream &operator<<(ostream &os, const Payment &p)
    {
        os << "Payment[ID:" << p.paymentId << ", Student:" << p.studentId << ", Amount:" << p.amount << "]";
        return os;
    }

    string serialize() const
    {
        return paymentId + "|" + studentId + "|" + monthYear + "|" + to_string(amount) + "|" + status + "|" + date;
    }

    static Payment deserialize(const string &line)
    {
        vector<string> parts;
        stringstream ss(line);
        string temp;
        while (getline(ss, temp, '|'))
            parts.push_back(temp);

        string pid = parts.size() > 0 ? parts[0] : "";
        string sid = parts.size() > 1 ? parts[1] : "";
        string my = parts.size() > 2 ? parts[2] : "";
        double amt = parts.size() > 3 ? stod(parts[3]) : 0;
        string st = parts.size() > 4 ? parts[4] : "pending";
        string d = parts.size() > 5 ? parts[5] : "";
        return Payment(pid, sid, my, amt, st, d);
    }

    void display() const
    {
        cout << CYAN << "  💰 Payment[" << RESET << paymentId << CYAN << "] " << RESET;
        cout << "Student: " << YELLOW << studentId << RESET;
        cout << " | Month: " << BLUE << monthYear << RESET;
        cout << " | Amount: " << GREEN << amount << " Tk" << RESET;
        cout << " | Status: " << (status == "paid" ? GREEN : RED) << status << RESET << "\n";
    }
};

class Student : public Person
{
private:
    string *dept;
    string *batch;
    string *roomNo;
    string *email;
    string *age;
    string *homeDistrict;
    string *college;
    string *school;

public:
    // Default Constructor
    Student(string id_ = "", string name_ = "", string dept_ = "", string batch_ = "", string roomNo_ = "",
            string email_ = "", string age_ = "", string homeDistrict_ = "", string college_ = "", string school_ = "")
        : Person(name_, id_)
    {

        // Dynamic memory allocation
        dept = new string(dept_);
        batch = new string(batch_);
        roomNo = new string(roomNo_);
        email = new string(email_);
        age = new string(age_);
        homeDistrict = new string(homeDistrict_);
        college = new string(college_);
        school = new string(school_);
    }

    // Copy Constructor
    Student(const Student &other) : Person(other.name, other.id)
    {
        dept = new string(*other.dept);
        batch = new string(*other.batch);
        roomNo = new string(*other.roomNo);
        email = new string(*other.email);
        age = new string(*other.age);
        homeDistrict = new string(*other.homeDistrict);
        college = new string(*other.college);
        school = new string(*other.school);
    }

    // Assignment Operator
    Student &operator=(const Student &other)
    {
        if (this != &other)
        {
            // Copy base class members
            name = other.name;
            id = other.id;

            // Copy derived class members with deep copy
            *dept = *other.dept;
            *batch = *other.batch;
            *roomNo = *other.roomNo;
            *email = *other.email;
            *age = *other.age;
            *homeDistrict = *other.homeDistrict;
            *college = *other.college;
            *school = *other.school;
        }
        return *this;
    }

    // Destructor
    ~Student()
    {
        delete dept;
        delete batch;
        delete roomNo;
        delete email;
        delete age;
        delete homeDistrict;
        delete college;
        delete school;
    }

    // Getters
    string getRoom() const { return *roomNo; }
    string getDept() const { return *dept; }
    string getBatch() const { return *batch; }
    string getEmail() const { return *email; }
    string getAge() const { return *age; }
    string getHomeDistrict() const { return *homeDistrict; }
    string getCollege() const { return *college; }
    string getSchool() const { return *school; }

    // Setters
    void setRoom(const string &r) { *roomNo = r; }
    void setDept(const string &d) { *dept = d; }
    void setBatch(const string &b) { *batch = b; }
    void setEmail(const string &e) { *email = e; }
    void setAge(const string &a) { *age = a; }
    void setHomeDistrict(const string &h) { *homeDistrict = h; }
    void setCollege(const string &c) { *college = c; }
    void setSchool(const string &s) { *school = s; }

    void display() const override
    {
        cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        cout << "║                    STUDENT PROFILE                         ║\n";
        cout << "╚════════════════════════════════════════════════════════════╝\n\n";

        cout << "  📋 Basic Information:\n";
        cout << "  ├─ Student ID    : " << id << "\n";
        cout << "  ├─ Name          : " << name << "\n";
        cout << "  ├─ Department    : " << *dept << "\n";
        cout << "  ├─ Batch         : " << *batch << "\n";
        if (!(*age).empty())
            cout << "  ├─ Age           : " << *age << "\n";
        cout << "  └─ Email         : " << ((*email).empty() ? "Not set" : *email) << "\n\n";

        cout << "  🏠 Accommodation:\n";
        cout << "  └─ Room Number   : " << ((*roomNo).empty() ? "Not assigned" : *roomNo) << "\n\n";

        if (!(*homeDistrict).empty() || !(*college).empty() || !(*school).empty())
        {
            cout << "  📍 Additional Information:\n";
            if (!(*homeDistrict).empty())
                cout << "  ├─ Home District : " << *homeDistrict << "\n";
            if (!(*college).empty())
                cout << "  ├─ College       : " << *college << "\n";
            if (!(*school).empty())
                cout << "  └─ School        : " << *school << "\n";
            cout << "\n";
        }

        cout << "════════════════════════════════════════════════════════════\n";
    }

    string serialize() const
    {
        return id + "|" + name + "|" + *dept + "|" + *batch + "|" +
               ((*roomNo).empty() ? "-" : *roomNo) + "|" +
               ((*email).empty() ? "-" : *email) + "|" +
               ((*age).empty() ? "-" : *age) + "|" +
               ((*homeDistrict).empty() ? "-" : *homeDistrict) + "|" +
               ((*college).empty() ? "-" : *college) + "|" +
               ((*school).empty() ? "-" : *school);
    }

    static Student deserialize(const string &line)
    {
        vector<string> t;
        string cur;
        for (char c : line)
        {
            if (c == '|')
            {
                t.push_back(cur);
                cur = "";
            }
            else
                cur += c;
        }
        t.push_back(cur);

        return Student(
            t.size() > 0 ? t[0] : "",
            t.size() > 1 ? t[1] : "",
            t.size() > 2 ? t[2] : "",
            t.size() > 3 ? t[3] : "",
            (t.size() > 4 && t[4] != "-") ? t[4] : "",
            (t.size() > 5 && t[5] != "-") ? t[5] : "",
            (t.size() > 6 && t[6] != "-") ? t[6] : "",
            (t.size() > 7 && t[7] != "-") ? t[7] : "",
            (t.size() > 8 && t[8] != "-") ? t[8] : "",
            (t.size() > 9 && t[9] != "-") ? t[9] : "");
    }

    bool operator==(const Student &other) const
    {
        return id == other.id;
    }

    bool operator==(const string &searchId) const
    {
        return id == searchId;
    }

    bool operator<(const Student &other) const
    {
        return id < other.id;
    }

    bool operator>(const Student &other) const
    {
        return name > other.name;
    }

    friend ostream &operator<<(ostream &os, const Student &s)
    {
        os << "Student[ID:" << s.id << ", Name:" << s.name << ", Dept:" << *s.dept << "]";
        return os;
    }
};

// ==================== ENHANCED STUDENT PROFILE VIEW ====================

void displayStudentProfileEnhanced(const Student &s)
{
    clearScreen();

    cout << MAGENTA << BOLD;
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

    string title = "👤 STUDENT PROFILE";
    int padding = (64 - title.length()) / 2;
    cout << "║" << string(padding, ' ') << title
         << string(64 - padding - title.length(), ' ') << "  ║\n";

    cout << "╚════════════════════════════════════════════════════════════════╝\n"
         << RESET;

    cout << "\n"
         << CYAN << BOLD << "  📋 Basic Information:\n"
         << RESET;
    cout << "  ├─ " << YELLOW << "Student ID    : " << RESET << s.getId() << "\n";
    cout << "  ├─ " << YELLOW << "Name          : " << RESET << s.getName() << "\n";
    cout << "  ├─ " << YELLOW << "Department    : " << RESET << s.getDept() << "\n";
    cout << "  ├─ " << YELLOW << "Batch         : " << RESET << s.getBatch() << "\n";
    if (!s.getAge().empty())
        cout << "  ├─ " << YELLOW << "Age           : " << RESET << s.getAge() << "\n";
    cout << "  └─ " << YELLOW << "Email         : " << RESET << (s.getEmail().empty() ? "Not set" : s.getEmail()) << "\n";

    cout << "\n"
         << CYAN << BOLD << "  🏠 Accommodation:\n"
         << RESET;
    if (s.getRoom().empty())
        cout << "  └─ " << RED << "Room Number   : Not assigned ✗" << RESET << "\n";
    else
        cout << "  └─ " << GREEN << "Room Number   : " << s.getRoom() << " ✓" << RESET << "\n";

    if (!s.getHomeDistrict().empty() || !s.getCollege().empty() || !s.getSchool().empty())
    {
        cout << "\n"
             << CYAN << BOLD << "  📍 Additional Information:\n"
             << RESET;
        if (!s.getHomeDistrict().empty())
            cout << "  ├─ " << YELLOW << "Home District : " << RESET << s.getHomeDistrict() << "\n";
        if (!s.getCollege().empty())
            cout << "  ├─ " << YELLOW << "College       : " << RESET << s.getCollege() << "\n";
        if (!s.getSchool().empty())
            cout << "  └─ " << YELLOW << "School        : " << RESET << s.getSchool() << "\n";
    }

    printSeparator();
}

// ==================== ENHANCED TABLE VIEW ====================
void displayStudentsTableEnhanced(const vector<Student> &students)
{
    clearScreen();

    // Modern Header
    cout << BLUE << BOLD;
    cout << "\n    ╔════════════════════════════════════════════════════════════════╗\n";
    cout << "    ║                    📋 STUDENTS DIRECTORY                       ║\n";
    cout << "    ╚════════════════════════════════════════════════════════════════╝\n"
         << RESET;

    if (students.empty())
    {
        printWarning("No students found in the system.");
        return;
    }

    // Modern Table Design
    cout << MAGENTA << "  ╔════════════╦══════════════════════╦════════════╦════════╦════════════╗\n"
         << RESET;
    cout << MAGENTA << "  ║ " << CYAN << BOLD << "Student ID " << MAGENTA << "║ " << CYAN << BOLD << "Name               " << MAGENTA << "  ║ " << CYAN << BOLD << "Department " << MAGENTA << "║ " << CYAN << BOLD << "Batch " << MAGENTA << " ║ " << CYAN << BOLD << "Room      " << MAGENTA << " ║\n"
         << RESET;
    cout << MAGENTA << "  ╠════════════╬══════════════════════╬════════════╬════════╬════════════╣\n"
         << RESET;

    for (const auto &s : students)
    {
        string nameDisplay = s.getName();
        if (nameDisplay.length() > 18)
        {
            nameDisplay = nameDisplay.substr(0, 17) + ".";
        }

        string roomDisplay = s.getRoom().empty() ? "None" : s.getRoom();
        string roomColor = s.getRoom().empty() ? RED : GREEN;
        string roomIcon = s.getRoom().empty() ? "🚫" : "✅";

        cout << MAGENTA << "  ║ " << RESET
             << YELLOW << setw(11) << left << s.getId() << MAGENTA << "║ " << RESET
             << WHITE << setw(20) << left << nameDisplay << MAGENTA << " ║ " << RESET
             << BLUE << setw(11) << left << s.getDept() << MAGENTA << "║ " << RESET
             << GREEN << setw(7) << left << s.getBatch() << MAGENTA << "║ " << RESET
             << roomColor << roomIcon << " " << setw(8) << left << roomDisplay << MAGENTA << "║\n"
             << RESET;
    }

    cout << MAGENTA << "  ╚════════════╩══════════════════════╩════════════╩════════╩════════════╝\n"
         << RESET;

    // Enhanced Statistics
    int assigned = count_if(students.begin(), students.end(),
                            [](const Student &s)
                            { return !s.getRoom().empty(); });

    cout << "\n  " << CYAN << "📈 QUICK STATS: " << RESET
         << GREEN << "🏠 " << assigned << " with rooms" << RESET
         << " • " << RED << "🚫 " << (students.size() - assigned) << " without rooms" << RESET
         << " • " << YELLOW << "👥 " << students.size() << " total students" << RESET << "\n";

    printSeparator();
}

class Room
{
public:
    string roomNo;
    int capacity;
    vector<string> occupants;

    Room(string rn = "", int cap = 1) : roomNo(rn), capacity(cap) {}
    bool hasSpace() const { return (int)occupants.size() < capacity; }

    // OPERATOR OVERLOADING
    bool operator==(const Room &other) const
    {
        return roomNo == other.roomNo;
    }

    bool operator==(const string &roomNumber) const
    {
        return roomNo == roomNumber;
    }

    bool operator<(const Room &other) const
    {
        return roomNo < other.roomNo;
    }

    bool operator>(const Room &other) const
    {
        return capacity > other.capacity;
    }

    // Bool conversion for availability check
    operator bool() const
    {
        return hasSpace();
    }

    // Output operator
    friend ostream &operator<<(ostream &os, const Room &r)
    {
        os << "Room[" << r.roomNo << ", Capacity:" << r.capacity << ", Occupied:" << r.occupants.size() << "]";
        return os;
    }

    string serialize() const
    {
        string occ = "-";
        if (!occupants.empty())
        {
            occ = occupants[0];
            for (size_t i = 1; i < occupants.size(); ++i)
                occ += "," + occupants[i];
        }
        return roomNo + "|" + to_string(capacity) + "|" + occ;
    }

    static Room deserialize(const string &line)
    {
        vector<string> t;
        string cur;
        for (char c : line)
        {
            if (c == '|')
            {
                t.push_back(cur);
                cur = "";
            }
            else
                cur += c;
        }
        t.push_back(cur);
        string rn = t.size() > 0 ? t[0] : "";
        int cap = t.size() > 1 ? stoi(t[1]) : 1;
        Room r(rn, cap);
        if (t.size() > 2 && t[2] != "-")
        {
            stringstream ss(t[2]);
            string id;
            while (getline(ss, id, ','))
                r.occupants.push_back(id);
        }
        return r;
    }

    void display() const
    {
        cout << "Room: " << roomNo << " | Capacity: " << capacity << " | Occupied: " << occupants.size() << "\n";
        if (!occupants.empty())
        {
            cout << "  Occupants IDs: ";
            for (size_t i = 0; i < occupants.size(); ++i)
            {
                if (i)
                    cout << ", ";
                cout << occupants[i];
            }
            cout << "\n";
        }
    }
};

class Complaint
{
public:
    string complaintId, studentId, text, status, adminReply; // Added adminReply

    Complaint(string cid = "", string sid = "", string txt = "", string st = "pending", string reply = "")
        : complaintId(cid), studentId(sid), text(txt), status(st), adminReply(reply) {}

    // OPERATOR OVERLOADING - FIXED
    bool operator==(const Complaint &other) const
    {
        return complaintId == other.complaintId;
    }

    // Compare with complaint ID
    bool operator==(const string &searchComplaintId) const
    {
        return complaintId == searchComplaintId;
    }

    // Compare with student ID - use different function
    bool hasStudentId(const string &searchStudentId) const
    {
        return studentId == searchStudentId;
    }

    // Compare complaint status
    bool hasStatus(const string &statusStr) const
    {
        return status == statusStr;
    }

    bool operator<(const Complaint &other) const
    {
        return complaintId < other.complaintId;
    }

    // Output operator
    friend ostream &operator<<(ostream &os, const Complaint &c)
    {
        os << "Complaint[ID:" << c.complaintId << ", Student:" << c.studentId << ", Status:" << c.status << "]";
        return os;
    }

    string serialize() const
    {
        string escText = text;
        string escReply = adminReply;
        for (char &c : escText)
            if (c == '|')
                c = '/';
        for (char &c : escReply)
            if (c == '|')
                c = '/';
        return complaintId + "|" + studentId + "|" + escText + "|" + status + "|" + escReply;
    }

    static Complaint deserialize(const string &line)
    {
        vector<string> t;
        string cur;
        for (char c : line)
        {
            if (c == '|')
            {
                t.push_back(cur);
                cur = "";
            }
            else
                cur += c;
        }
        t.push_back(cur);
        return Complaint(
            t.size() > 0 ? t[0] : "",
            t.size() > 1 ? t[1] : "",
            t.size() > 2 ? t[2] : "",
            t.size() > 3 ? t[3] : "pending",
            t.size() > 4 ? t[4] : "");
    }

    void display() const
    {
        cout << YELLOW << "  📢 Complaint[" << RESET << complaintId << YELLOW << "] " << RESET;
        cout << "Student: " << CYAN << studentId << RESET;
        cout << " | Status: " << (status == "pending" ? RED : GREEN) << status << RESET << "\n";
        cout << "     📝 " << text << "\n";
        if (!adminReply.empty())
            cout << "     💬 Admin: " << GREEN << adminReply << RESET << "\n\n";
        else
            cout << "\n\n";
    }
};

class SwapRequest
{
public:
    string requestId, fromStudentId, toStudentId, status;

    SwapRequest(string rid = "", string from = "", string to = "", string st = "pending")
        : requestId(rid), fromStudentId(from), toStudentId(to), status(st) {}

    string serialize() const
    {
        return requestId + "|" + fromStudentId + "|" + toStudentId + "|" + status;
    }

    static SwapRequest deserialize(const string &line)
    {
        vector<string> t;
        string cur;
        for (char c : line)
        {
            if (c == '|')
            {
                t.push_back(cur);
                cur = "";
            }
            else
                cur += c;
        }
        t.push_back(cur);
        return SwapRequest(
            t.size() > 0 ? t[0] : "",
            t.size() > 1 ? t[1] : "",
            t.size() > 2 ? t[2] : "",
            t.size() > 3 ? t[3] : "pending");
    }

    void display() const
    {
        cout << BLUE << "  🔄 SwapRequest[" << RESET << requestId << BLUE << "] " << RESET;
        cout << "From: " << YELLOW << fromStudentId << RESET;
        cout << " → To: " << YELLOW << toStudentId << RESET;
        cout << " | Status: " << (status == "pending" ? YELLOW : (status == "accepted" ? GREEN : RED)) << status << RESET << "\n";
    }
};

// ------------------ HallSystem ------------------

class HallSystem
{
private:
    vector<Student> students;
    vector<Room> rooms;
    vector<Complaint> complaints;
    vector<SwapRequest> swapRequests;
    vector<MealPlan> mealPlans;
    vector<DailyMenu> menus;
    vector<Payment> payments;

    string studentsFile = "students.txt";
    string roomsFile = "rooms.txt";
    string complaintsFile = "complaints.txt";
    string swapRequestsFile = "swap_requests.txt";
    string mealPlansFile = "meal_plans.txt";
    string menusFile = "menus.txt";
    string paymentsFile = "payments.txt";

    LoginManager lm{LOGIN_FILE};

    template <typename T, typename Key>
    T *findById(vector<T> &container, const Key &id)
    {
        for (auto &item : container)
        {
            if (item == id)
            { // Uses overloaded == operator
                return &item;
            }
        }
        return nullptr;
    }

    Student *findStudentById(const string &id)
    {
        return findById<Student, string>(students, id);
    }

    Room *findRoomByNo(const string &rno)
    {
        return findById<Room, string>(rooms, rno);
    }

    MealPlan *findMealPlanByStudentId(const string &sid)
    {
        return findById<MealPlan, string>(mealPlans, sid);
    }

    DailyMenu *findMenuByDate(const string &date)
    {
        return findById<DailyMenu, string>(menus, date);
    }

    // Forward declarations of private methods
    void changeStudentPassword(const string &sid);
    void setSecurityQuestion(const string &sid);
    void editStudentProfile(const string &sid);
    void requestRoomSwap(const string &sid);
    void viewSwapRequests(const string &sid);
    void processSwapRequest(const string &sid, const string &requestId, bool accept);
    void manageMealPlan(const string &sid);
    void markMealOff(const string &sid);
    void viewMealPlan(const string &sid);
    void makePayment(const string &sid);
    void administrationUpdateMenu();
    void administrationViewPayments();
    void viewTodayMenu(const string &sid);
    void administrationCalculateAndGenerateBill();
    void administrationSearchPayment();
    void studentViewPaymentSlips(const string &sid);
    void updateStudentInLoginFile(const string &sid, const string &field, const string &value);

public:
    template <typename Container>
    void displayContainer(const Container &container, const string &title)
    {
        clearScreen();
        printHeader(title);

        if (container.empty())
        {
            printWarning("No items found.");
            return;
        }

        cout << "\n";
        for (const auto &item : container)
        {
            item.display();
        }

        printSeparator();
        cout << GREEN << "\n   📊 Total Items: " << container.size() << RESET << "\n";
    }

    HallSystem() { loadAll(); }

    // ------------------ Student CRUD Functions ------------------

    void addStudent();
    void viewStudents()
    {
        displayContainer(students, "STUDENTS LIST");
        pauseAndClear();
    }
    void deleteStudent();
    void deleteAdministration()
    {
        clearScreen();
        printHeader("DELETE ADMINISTRATION ACCOUNT");

        printWarning("⚠️  DANGER ZONE - ADMINISTRATION ACCOUNT DELETION");
        cout << "\n";

        // Show all administrations
        cout << CYAN << BOLD << "  📋 Current Administration Accounts:\n"
             << RESET;
        printSeparator();

        vector<string> headers = {"Administration ID", "Name", "Email"};
        vector<int> widths = {20, 25, 30};
        printTableHeader(headers, widths);

        int administrationCount = 0;
        ifstream fin(LOGIN_FILE);
        string line;
        vector<tuple<string, string, string>> administrationList; // id, name, email

        while (getline(fin, line))
        {
            if (line.find("administration|") == 0)
            {
                stringstream ss(line);
                string parts[7];
                for (int i = 0; i < 7; i++)
                    getline(ss, parts[i], '|');

                administrationList.push_back(make_tuple(parts[2], parts[1], parts[4]));
                administrationCount++;

                vector<string> row = {parts[2], parts[1], parts[4]};
                printTableRow(row, widths);
            }
        }
        fin.close();

        printSeparator();
        cout << YELLOW << "\n  Total Administrations: " << administrationCount << RESET << "\n\n";

        if (administrationCount <= 1)
        {
            printError("Cannot delete! At least one administration must remain in the system.");
            printBox("System requires minimum 1 administration account 🔒");
            pauseAndClear();
            return;
        }

        printWarning("Enter Administration ID to delete (or type '0' to cancel): ");
        cout << YELLOW << "  » " << RESET;
        string id;
        cin >> ws;
        getline(cin, id);

        if (id == "0")
        {
            printInfo("Deletion cancelled.");
            pauseAndClear();
            return;
        }

        // Verify administration exists
        bool administrationExists = false;
        for (const auto &administration : administrationList)
        {
            if (get<0>(administration) == id)
            {
                administrationExists = true;
                break;
            }
        }

        if (!administrationExists)
        {
            printError("Administration ID not found!");
            pauseAndClear();
            return;
        }

        cout << "\n";
        printWarning("Enter Administration Password for confirmation: ");
        cout << YELLOW << "  » " << RESET;
        string pass = readPassword();

        bool verified = false;
        for (const auto &l : lm.logins)
        {
            if (l.getRole() == "administration" && l.getId() == id && l.getPassword() == pass)
            {
                verified = true;
                break;
            }
        }

        if (!verified)
        {
            printError("Invalid credentials! Deletion cancelled.");
            printBox("Authentication failed ❌");
            pauseAndClear();
            return;
        }

        cout << "\n";
        cout << RED << BOLD << "  ⚠️  FINAL CONFIRMATION ⚠️\n"
             << RESET;
        cout << RED << "  This action CANNOT be undone!\n"
             << RESET;
        cout << YELLOW << "  Type 'DELETE' in capital letters to confirm: " << RESET;
        string confirm;
        getline(cin, confirm);

        if (confirm == "DELETE")
        {
            showLoadingAnimation("Deleting administration account", 2);

            vector<string> lines;
            ifstream finRead(LOGIN_FILE);
            while (getline(finRead, line))
            {
                if (line.find("administration|") == 0)
                {
                    stringstream ss(line);
                    string parts[7];
                    for (int i = 0; i < 7; i++)
                        getline(ss, parts[i], '|');

                    if (parts[2] != id)
                        lines.push_back(line);
                }
                else
                {
                    lines.push_back(line);
                }
            }
            finRead.close();

            ofstream fout(LOGIN_FILE);
            for (const auto &l : lines)
                fout << l << "\n";
            fout.close();

            lm.loadLogins();

            clearScreen();
            printSuccess("Administration account deleted successfully!");
            printBox("Account removed from system 🗑️");
            cout << "\n";
            printInfo("Logging out for security reasons...");

            pauseAndClear();
            return;
        }
        else
        {
            printInfo("Deletion cancelled - confirmation text didn't match.");
            pauseAndClear();
        }
    }

    // ------------------ Room Functions ------------------
    void addRoom();
    void viewRooms()
    {
        displayContainer(rooms, "ROOMS LIST");
        pauseAndClear();
    }
    void assignRoomMenu()
    {
        while (true)
        {
            clearScreen();

            // Beautiful header design
            cout << MAGENTA << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

            string title = "🏠 ROOM ASSIGNMENT";
            int titlePadding = (64 - title.length()) / 2;
            cout << "║" << string(titlePadding, ' ') << title
                 << string(64 - titlePadding - title.length(), ' ') << "  ║\n";

            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            // Quick stats
            cout << "\n"
                 << CYAN << BOLD << "  📊 Quick Statistics:\n"
                 << RESET;

            int totalStudents = students.size();
            int assignedStudents = 0;
            int unassignedStudents = 0;

            for (const auto &s : students)
            {
                if (!s.getRoom().empty())
                    assignedStudents++;
                else
                    unassignedStudents++;
            }

            int totalRooms = rooms.size();
            int availableRooms = 0;
            int fullRooms = 0;

            for (const auto &r : rooms)
            {
                if (r.hasSpace())
                    availableRooms++;
                else
                    fullRooms++;
            }

            cout << "  ├─ " << YELLOW << "Students: " << RESET << assignedStudents << GREEN << " assigned" << RESET
                 << " | " << unassignedStudents << RED << " unassigned" << RESET << "\n";
            cout << "  └─ " << YELLOW << "Rooms: " << RESET << availableRooms << GREEN << " available" << RESET
                 << " | " << fullRooms << RED << " full" << RESET << "\n";

            printSeparator();

            // Menu options with beautiful icons and descriptions
            cout << "\n"
                 << CYAN << BOLD << "  🎯 Assignment Methods:\n"
                 << RESET;

            printMenuOption(1, "👤 Manual Assignment", false);

            printMenuOption(2, "📚 Auto Assignment by Batch", false);

            printMenuOption(3, "🎓 Auto Assignment by Department", false);

            printMenuOption(4, "🚀 First Available Room", false);

            printMenuOption(5, "⚡ Auto Assign ALL Unassigned", false);

            cout << "\n"
                 << CYAN << BOLD << "  🔙 Navigation:\n"
                 << RESET;
            printMenuOption(6, "← Back to Administration Menu", true);

            printSeparator();

            // Input section with validation
            cout << YELLOW << "\n   🎮 Enter your choice (1-6): " << RESET;

            int choice;
            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Show error with style
                clearScreen();
                cout << RED << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                      ❌ INVALID INPUT                          ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;
                cout << "\n"
                     << YELLOW << "   Please enter a number between 1 and 6." << RESET << "\n";

                // Wait and continue
                cout << "\n"
                     << CYAN << "   Press Enter to continue..." << RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Process choice with loading animation
            switch (choice)
            {
            case 1:
                showLoadingAnimation("Loading Manual Assignment", 1);
                assignRoom();
                break;
            case 2:
            case 3:
            case 4:
            case 5:
                showLoadingAnimation("Preparing Auto Assignment", 1);
                autoAssignRoom(choice - 1);
                break;
            case 6:
                // Elegant exit
                clearScreen();
                printSuccess("Returning to Administration Menu...");
                pauseAndClear();
                return;
            default:
                // Handle invalid choice
                clearScreen();
                cout << RED << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                    ❌ INVALID CHOICE                          ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;
                cout << "\n"
                     << YELLOW << "   Please select a valid option (1-6)." << RESET << "\n";

                cout << "\n"
                     << CYAN << "   Press Enter to continue..." << RESET;
                cin.get();
                break;
            }
        }
    }

    void assignRoom();
    void autoAssignRoom(int strategy)
    {
        clearScreen();
        cout << "--- Automatic Room Assignment ---\n";

        if (strategy == 4)
        {
            cout << "\nAssigning ALL unassigned students...\n\n";
            int assignedCount = 0;

            for (auto &student : students)
            {
                if (student.getRoom().empty())
                {
                    Room *bestRoom = nullptr;

                    for (auto &room : rooms)
                    {
                        if (room.hasSpace())
                        {
                            bool sameBatch = false;
                            for (const auto &occupantId : room.occupants)
                            {
                                Student *occupant = findStudentById(occupantId);
                                if (occupant && occupant->getBatch() == student.getBatch())
                                {
                                    sameBatch = true;
                                    break;
                                }
                            }

                            if (sameBatch || room.occupants.empty())
                            {
                                bestRoom = &room;
                                break;
                            }
                        }
                    }

                    if (!bestRoom)
                    {
                        for (auto &room : rooms)
                        {
                            if (room.hasSpace())
                            {
                                bestRoom = &room;
                                break;
                            }
                        }
                    }

                    if (bestRoom)
                    {
                        bestRoom->occupants.push_back(student.getId());
                        student.setRoom(bestRoom->roomNo);
                        assignedCount++;
                        cout << "✓ " << student.getName() << " (ID: " << student.getId()
                             << ") -> Room " << bestRoom->roomNo << "\n";
                    }
                }
            }

            cout << "\n════════════════════════════════════════\n";
            cout << "✓ Successfully assigned " << assignedCount << " students!\n";

            int unassigned = 0;
            for (const auto &s : students)
            {
                if (s.getRoom().empty())
                    unassigned++;
            }

            if (unassigned > 0)
                cout << "⚠ " << unassigned << " students remain unassigned (No available rooms)\n";

            pauseAndClear();
            return;
        }

        cout << "\nEnter Student ID to assign: ";
        string sid;
        getline(cin, sid);

        Student *s = findStudentById(sid);
        if (!s)
        {
            cout << "Student not found.\n";
            pauseAndClear();
            return;
        }

        if (!s->getRoom().empty())
        {
            cout << "\nStudent already has room: " << s->getRoom() << "\n";
            cout << "Do you want to reassign? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice != 'y' && choice != 'Y')
            {
                pauseAndClear();
                return;
            }
        }

        Room *assignedRoom = nullptr;

        if (strategy == 1)
        {
            cout << "\nSearching for room with batch: " << s->getBatch() << "...\n";

            for (auto &room : rooms)
            {
                if (room.hasSpace())
                {
                    bool sameBatch = false;
                    for (const auto &occupantId : room.occupants)
                    {
                        Student *occupant = findStudentById(occupantId);
                        if (occupant && occupant->getBatch() == s->getBatch())
                        {
                            sameBatch = true;
                            break;
                        }
                    }

                    if (sameBatch || room.occupants.empty())
                    {
                        assignedRoom = &room;
                        break;
                    }
                }
            }
        }
        else if (strategy == 2)
        {
            cout << "\nSearching for room with department: " << s->getDept() << "...\n";

            for (auto &room : rooms)
            {
                if (room.hasSpace())
                {
                    bool sameDept = false;
                    for (const auto &occupantId : room.occupants)
                    {
                        Student *occupant = findStudentById(occupantId);
                        if (occupant && occupant->getDept() == s->getDept())
                        {
                            sameDept = true;
                            break;
                        }
                    }

                    if (sameDept || room.occupants.empty())
                    {
                        assignedRoom = &room;
                        break;
                    }
                }
            }
        }
        else if (strategy == 3)
        {
            cout << "\nSearching for first available room...\n";

            for (auto &room : rooms)
            {
                if (room.hasSpace())
                {
                    assignedRoom = &room;
                    break;
                }
            }
        }

        if (assignedRoom)
        {
            string prevRoom = s->getRoom();
            if (!prevRoom.empty())
            {
                Room *pr = findRoomByNo(prevRoom);
                if (pr)
                    pr->occupants.erase(remove(pr->occupants.begin(), pr->occupants.end(), sid), pr->occupants.end());
            }

            assignedRoom->occupants.push_back(sid);
            s->setRoom(assignedRoom->roomNo);

            cout << "\n✓ Successfully assigned to Room: " << assignedRoom->roomNo << "\n";
            cout << "  Current Occupancy: " << assignedRoom->occupants.size()
                 << "/" << assignedRoom->capacity << "\n";
        }
        else
        {
            cout << "\n✗ No suitable room found!\n";
            cout << "  Reasons: All rooms are full or no matching criteria found.\n";
        }

        pauseAndClear();
    }

    // ------------------ Complaint Functions ------------------
    void submitComplaint(const string &studentId);
    void viewComplaints()
    {
        displayContainer(complaints, "COMPLAINTS LIST");
        pauseAndClear();
    }
    void viewComplaintStatus(const string &sid);
    void replyComplaint();
    // ------------------ File I/O ------------------
    void loadAll()
    {
        students.clear();
        rooms.clear();
        complaints.clear();
        swapRequests.clear();
        mealPlans.clear();
        menus.clear();
        payments.clear();

        ifstream fs(studentsFile);
        string line;
        while (fs.is_open() && getline(fs, line))
            if (!line.empty())
                students.push_back(Student::deserialize(line));
        fs.close();

        ifstream fr(roomsFile);
        while (fr.is_open() && getline(fr, line))
            if (!line.empty())
                rooms.push_back(Room::deserialize(line));
        fr.close();

        ifstream fc(complaintsFile);
        while (fc.is_open() && getline(fc, line))
            if (!line.empty())
                complaints.push_back(Complaint::deserialize(line));
        fc.close();

        ifstream fsw(swapRequestsFile);
        while (fsw.is_open() && getline(fsw, line))
            if (!line.empty())
                swapRequests.push_back(SwapRequest::deserialize(line));
        fsw.close();

        ifstream fmp(mealPlansFile);
        while (fmp.is_open() && getline(fmp, line))
            if (!line.empty())
                mealPlans.push_back(MealPlan::deserialize(line));
        fmp.close();

        ifstream fm(menusFile);
        while (fm.is_open() && getline(fm, line))
            if (!line.empty())
                menus.push_back(DailyMenu::deserialize(line));
        fm.close();

        ifstream fp(paymentsFile);
        while (fp.is_open() && getline(fp, line))
            if (!line.empty())
                payments.push_back(Payment::deserialize(line));
        fp.close();
    }

    void saveAll()
    {
        ofstream fs(studentsFile);
        for (const auto &s : students)
            fs << s.serialize() << "\n";
        fs.close();

        ofstream fr(roomsFile);
        for (const auto &r : rooms)
            fr << r.serialize() << "\n";
        fr.close();

        ofstream fc(complaintsFile);
        for (const auto &c : complaints)
            fc << c.serialize() << "\n";
        fc.close();

        ofstream fsw(swapRequestsFile);
        for (const auto &sw : swapRequests)
            fsw << sw.serialize() << "\n";
        fsw.close();

        ofstream fmp(mealPlansFile);
        for (const auto &mp : mealPlans)
            fmp << mp.serialize() << "\n";
        fmp.close();

        ofstream fm(menusFile);
        for (const auto &m : menus)
            fm << m.serialize() << "\n";
        fm.close();

        ofstream fp(paymentsFile);
        for (const auto &p : payments)
            fp << p.serialize() << "\n";
        fp.close();
    }

    // ------------------ Menus ------------------

    void administrationMenu();
    void studentMenu(const string &sid);
    void showDashboardAnalytics();

    void start()
    {
        clearScreen();
        Registration reg(LOGIN_FILE);

        if (!reg.fileExists())
        {
            cout << "No administration found. Register first administration.\n";
            reg.registerAdministration();
        }

        lm.loadLogins();

        while (true)
        {
            displayMainMenu();

            int ch;
            if (!(cin >> ch))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ← এই line add করো

            switch (ch)
            {
            case 1:
                clearScreen(); // ← এই line add করো
                if (lm.administrationLogin())
                    administrationMenu();
                else
                {
                    cout << "Invalid ID or password.\n";
                    pauseAndClear();
                }
                break;

            case 2:
                clearScreen(); // ← এই line add করো
                reg.registerAdministration();
                lm.loadLogins();
                break;

            case 3:
            {
                clearScreen(); // ← এই line add করো
                string sid = lm.studentLogin();
                if (!sid.empty())
                    studentMenu(sid);
                else
                {
                    cout << "Invalid student login.\n";
                    pauseAndClear();
                }
                break;
            }

            case 4:
                clearScreen();
                lm.forgotPassword();
                break;

            case 5:
                saveAll();
                printGoodbye();
                exit(0);

            default:
                cout << "Invalid choice.\n";
                pauseAndClear();
            }
        }
    }
};

// ------------------ HallSystem Private Methods Implementation ------------------

void HallSystem::updateStudentInLoginFile(const string &sid, const string &field, const string &value)
{
    vector<string> lines;
    ifstream fin(LOGIN_FILE);
    string line;
    bool found = false;

    while (getline(fin, line))
    {
        if (line.find("student|") == 0)
        {
            stringstream ss(line);
            string parts[7];
            for (int i = 0; i < 7; i++)
                getline(ss, parts[i], '|');

            if (parts[2] == sid)
            {
                if (field == "name")
                    parts[1] = value;
                else if (field == "email")
                    parts[4] = value;

                line = parts[0] + "|" + parts[1] + "|" + parts[2] + "|" +
                       parts[3] + "|" + parts[4] + "|" + parts[5] + "|" + parts[6];
                found = true;
            }
        }
        lines.push_back(line);
    }
    fin.close();

    if (found)
    {
        ofstream fout(LOGIN_FILE);
        for (const auto &l : lines)
            fout << l << "\n";
        fout.close();
        lm.loadLogins();
    }
}

void HallSystem::changeStudentPassword(const string &sid)
{
    string currentPass, newPass;

    printHeader("CHANGE PASSWORD");

    // cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // ✅ ADD: Current password verification
    cout << YELLOW << "\n  Enter current password: " << RESET;
    currentPass = readPassword();

    // Verify current password
    bool verified = false;
    for (const auto &l : lm.logins)
    {
        if (l.getRole() == "student" && l.getId() == sid && l.getPassword() == currentPass)
        {
            verified = true;
            break;
        }
    }

    if (!verified)
    {
        printError("Current password is incorrect!");
        pauseAndClear();
        return;
    }

    cout << YELLOW << "\n  Enter new password: " << RESET;
    // cin.ignore(numeric_limits<streamsize>::max(), '\n');
    newPass = readPassword();

    if (newPass.empty())
    {
        printError("Password cannot be empty!");
        pauseAndClear();
        return;
    }

    // ✅ ADD: Confirm new password
    cout << YELLOW << "  Confirm new password: " << RESET;
    string confirmPass = readPassword();

    if (newPass != confirmPass)
    {
        printError("Passwords don't match!");
        pauseAndClear();
        return;
    }

    showLoadingAnimation("Updating password", 1);

    vector<string> lines;
    ifstream fin(LOGIN_FILE);
    string line;
    bool found = false;

    while (getline(fin, line))
    {
        if (line.find("student|") == 0)
        {
            stringstream ss(line);
            string parts[7];
            for (int i = 0; i < 7; i++)
                getline(ss, parts[i], '|');

            if (parts[2] == sid)
            {
                parts[3] = newPass;
                line = parts[0] + "|" + parts[1] + "|" + parts[2] + "|" +
                       parts[3] + "|" + parts[4] + "|" + parts[5] + "|" + parts[6];
                found = true;
            }
        }
        lines.push_back(line);
    }
    fin.close();

    if (found)
    {
        ofstream fout(LOGIN_FILE);
        for (const auto &l : lines)
            fout << l << "\n";
        fout.close();

        lm.loadLogins();

        printSuccess("Password changed successfully!");
        printBox("Your account is now more secure! 🔒");
    }
    else
    {
        printError("Student not found in login file!");
    }

    pauseAndClear();
}

void HallSystem::editStudentProfile(const string &sid)
{
    Student *s = findStudentById(sid);
    if (!s)
    {
        printError("Student not found!");
        pauseAndClear();
        return;
    }

    while (true)
    {
        clearScreen();

        // Beautiful header design
        cout << MAGENTA << BOLD;
        cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

        string title = "✏️  EDIT PROFILE";
        int titlePadding = (64 - title.length()) / 2;
        cout << "║" << string(titlePadding, ' ') << title
             << string(64 - titlePadding - title.length(), ' ') << "     ║\n";

        cout << "╚════════════════════════════════════════════════════════════════╝\n"
             << RESET;

        cout << "\n"
             << CYAN << BOLD << "  🏷️  Basic Information:\n"
             << RESET;
        cout << "  ├─ " << YELLOW << "Student ID    : " << RESET << s->getId() << "\n";
        cout << "  ├─ " << YELLOW << "Name          : " << RESET << s->getName() << "\n";
        cout << "  ├─ " << YELLOW << "Department    : " << RESET << s->getDept() << "\n";
        cout << "  ├─ " << YELLOW << "Batch         : " << RESET << s->getBatch() << "\n";
        if (!s->getAge().empty())
            cout << "  ├─ " << YELLOW << "Age           : " << RESET << s->getAge() << "\n";
        cout << "  └─ " << YELLOW << "Email         : " << RESET << (s->getEmail().empty() ? RED "Not set" : GREEN + s->getEmail()) << RESET << "\n";

        cout << "\n"
             << CYAN << BOLD << "  🏠 Accommodation:\n"
             << RESET;
        if (s->getRoom().empty())
            cout << "  └─ " << RED << "Room Number   : Not assigned ✗" << RESET << "\n";
        else
            cout << "  └─ " << GREEN << "Room Number   : " << s->getRoom() << " ✓" << RESET << "\n";

        if (!s->getHomeDistrict().empty() || !s->getCollege().empty() || !s->getSchool().empty())
        {
            cout << "\n"
                 << CYAN << BOLD << "  📍 Additional Information:\n"
                 << RESET;
            if (!s->getHomeDistrict().empty())
                cout << "  ├─ " << YELLOW << "Home District : " << RESET << s->getHomeDistrict() << "\n";
            if (!s->getCollege().empty())
                cout << "  ├─ " << YELLOW << "College       : " << RESET << s->getCollege() << "\n";
            if (!s->getSchool().empty())
                cout << "  └─ " << YELLOW << "School        : " << RESET << s->getSchool() << "\n";
        }

        printSeparator();

        // Edit options with beautiful icons
        cout << "\n"
             << CYAN << BOLD << "  🛠️  EDIT OPTIONS:\n"
             << RESET;

        cout << "\n"
             << CYAN << BOLD << "  👤 Personal Details:\n"
             << RESET;
        printMenuOption(1, "📝 Edit Name", false);
        printMenuOption(2, "📧 Edit Email", false);
        printMenuOption(3, "🎂 Edit Age", false);

        cout << "\n"
             << CYAN << BOLD << "  📍 Background Information:\n"
             << RESET;
        printMenuOption(4, "🏠 Edit Home District", false);
        printMenuOption(5, "🎓 Edit College", false);
        printMenuOption(6, "📚 Edit School", false);

        cout << "\n"
             << CYAN << BOLD << "  🔐 Security Settings:\n"
             << RESET;
        printMenuOption(7, "🛡️ Set Security Question", false);

        cout << "\n"
             << CYAN << BOLD << "  🔙 Navigation:\n"
             << RESET;
        printMenuOption(8, "← Back to Student Menu", true);

        printSeparator();

        // Input section with validation
        cout << YELLOW << "\n   🎮 What would you like to edit? (1-8): " << RESET;

        int ch;
        if (!(cin >> ch))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Show error with style
            clearScreen();
            cout << RED << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                      ❌ INVALID INPUT                          ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;
            cout << "\n"
                 << YELLOW << "   Please enter a number between 1 and 8." << RESET << "\n";

            pauseAndClear();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string input;
        bool updated = false;

        switch (ch)
        {
        case 1:
            clearScreen();
            printHeader("📝 EDIT NAME  ");
            cout << YELLOW << "\n  Current Name: " << RESET << s->getName() << "\n";
            cout << YELLOW << "  Enter new name: " << RESET;
            getline(cin, input);
            if (input.empty())
            {
                printError("Name cannot be empty!");
                pauseAndClear();
                break;
            }
            s->setName(input);
            updateStudentInLoginFile(sid, "name", input);
            updated = true;
            break;

        case 2:
            clearScreen();
            printHeader("📧 EDIT EMAIL");
            cout << YELLOW << "\n  Current Email: " << RESET << (s->getEmail().empty() ? "Not set" : s->getEmail()) << "\n";
            while (true)
            {
                cout << YELLOW << "  Enter new email (must contain @gmail.com): " << RESET;
                getline(cin, input);
                if (input.empty())
                {
                    printError("Email cannot be empty!");
                    pauseAndClear();
                    break;
                }
                if (input.find("@gmail.com") != string::npos)
                {
                    s->setEmail(input);
                    updateStudentInLoginFile(sid, "email", input);
                    updated = true;
                    break;
                }
                else
                {
                    printError("Invalid email! Email must contain @gmail.com");
                }
            }
            break;

        case 3:
            clearScreen();
            printHeader("🎂 EDIT AGE");
            cout << YELLOW << "\n  Current Age: " << RESET << (s->getAge().empty() ? "Not set" : s->getAge()) << "\n";
            cout << YELLOW << "  Enter age: " << RESET;
            getline(cin, input);
            s->setAge(input);
            updated = true;
            break;

        case 4:
            clearScreen();
            printHeader("🏠 EDIT HOME DISTRICT");
            cout << YELLOW << "\n  Current Home District: " << RESET << (s->getHomeDistrict().empty() ? "Not set" : s->getHomeDistrict()) << "\n";
            cout << YELLOW << "  Enter home district: " << RESET;
            getline(cin, input);
            s->setHomeDistrict(input);
            updated = true;
            break;

        case 5:
            clearScreen();
            printHeader("🎓 EDIT COLLEGE");
            cout << YELLOW << "\n  Current College: " << RESET << (s->getCollege().empty() ? "Not set" : s->getCollege()) << "\n";
            cout << YELLOW << "  Enter college name: " << RESET;
            getline(cin, input);
            s->setCollege(input);
            updated = true;
            break;

        case 6:
            clearScreen();
            printHeader("📚 EDIT SCHOOL");
            cout << YELLOW << "\n  Current School: " << RESET << (s->getSchool().empty() ? "Not set" : s->getSchool()) << "\n";
            cout << YELLOW << "  Enter school name: " << RESET;
            getline(cin, input);
            s->setSchool(input);
            updated = true;
            break;

        case 7:
            setSecurityQuestion(sid);
            continue;

        case 8:
            clearScreen();
            printSuccess("Returning to Student Menu...");
            pauseAndClear();
            return;

        default:
            printError("Invalid choice!");
            pauseAndClear();
            continue;
        }

        if (updated)
        {
            showLoadingAnimation("Updating profile", 1);
            clearScreen();
            printSuccess("Profile updated successfully! ✅");
            printBox("Your changes have been saved! 💾");

            // Show what was updated
            cout << "\n"
                 << GREEN << "  ✓ Updated: ";
            switch (ch)
            {
            case 1:
                cout << "Name";
                break;
            case 2:
                cout << "Email";
                break;
            case 3:
                cout << "Age";
                break;
            case 4:
                cout << "Home District";
                break;
            case 5:
                cout << "College";
                break;
            case 6:
                cout << "School";
                break;
            }
            cout << RESET << "\n";
        }

        pauseAndClear();
    }
}

void HallSystem::setSecurityQuestion(const string &sid)
{
    clearScreen();
    cout << "--- Set Security Question ---\n";

    string question, answer;
    cout << "Enter security question (e.g., What is your pet's name?): ";
    getline(cin, question);

    if (question.empty())
    {
        cout << "Security question cannot be empty!\n";
        pauseAndClear();
        return;
    }

    cout << "Enter answer: ";
    getline(cin, answer);

    if (answer.empty())
    {
        cout << "Answer cannot be empty!\n";
        pauseAndClear();
        return;
    }

    vector<string> lines;
    ifstream fin(LOGIN_FILE);
    string line;
    bool found = false;

    while (getline(fin, line))
    {
        if (line.find("student|") == 0)
        {
            stringstream ss(line);
            string parts[7];
            for (int i = 0; i < 7; i++)
                getline(ss, parts[i], '|');

            if (parts[2] == sid)
            {
                parts[5] = question;
                parts[6] = answer;
                line = parts[0] + "|" + parts[1] + "|" + parts[2] + "|" +
                       parts[3] + "|" + parts[4] + "|" + parts[5] + "|" + parts[6];
                found = true;
            }
        }
        lines.push_back(line);
    }
    fin.close();

    if (found)
    {
        ofstream fout(LOGIN_FILE);
        for (const auto &l : lines)
            fout << l << "\n";
        fout.close();

        lm.loadLogins();
        cout << "\nSecurity question set successfully!\n";
    }
    else
    {
        cout << "\nError: Student not found in login file.\n";
    }

    pauseAndClear();
}

void HallSystem::requestRoomSwap(const string &sid)
{
    while (true)
    {
        clearScreen();

        // Beautiful header design
        cout << MAGENTA << BOLD;
        cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

        string title = "🔄 ROOM SWAP REQUEST";
        int titlePadding = (64 - title.length()) / 2;
        cout << "║" << string(titlePadding, ' ') << title
             << string(64 - titlePadding - title.length(), ' ') << "  ║\n";

        cout << "╚════════════════════════════════════════════════════════════════╝\n"
             << RESET;

        Student *s = findStudentById(sid);
        if (!s)
        {
            printError("Student not found!");
            pauseAndClear();
            return;
        }

        // Check if student has a room
        if (s->getRoom().empty())
        {
            cout << "\n"
                 << RED << BOLD << "  ⚠️  NO ROOM ASSIGNED\n"
                 << RESET;
            cout << YELLOW << "  You don't have a room assigned yet. Cannot request swap.\n"
                 << RESET;
            cout << "\n"
                 << CYAN << "  Please contact administration to get a room assignment first.\n"
                 << RESET;

            printSeparator();
            cout << YELLOW << "\n  Press Enter to return... " << RESET;
            cin.get();
            return;
        }

        // Display current room information
        cout << "\n"
             << CYAN << BOLD << "  🏠 YOUR CURRENT ROOM:\n"
             << RESET;

        Room *currentRoom = findRoomByNo(s->getRoom());
        if (currentRoom)
        {
            cout << "  ├─ " << GREEN << "Room Number: " << RESET << s->getRoom() << "\n";
            cout << "  ├─ " << YELLOW << "Capacity: " << RESET << currentRoom->capacity << "\n";
            cout << "  └─ " << BLUE << "Current Occupants: " << RESET << currentRoom->occupants.size() << "\n";

            // Show roommates
            if (!currentRoom->occupants.empty())
            {
                cout << "\n"
                     << CYAN << "  👥 Your Roommates:\n"
                     << RESET;
                for (const auto &occupantId : currentRoom->occupants)
                {
                    if (occupantId != sid)
                    {
                        Student *roommate = findStudentById(occupantId);
                        if (roommate)
                            cout << "     • " << roommate->getName() << " (" << occupantId << ")\n";
                    }
                }
            }
        }

        printSeparator();

        // Instructions
        cout << "\n"
             << YELLOW << BOLD << "  💡 How it works:\n"
             << RESET;
        cout << "  " << WHITE << "1. Enter the Student ID you want to swap with\n";
        cout << "  " << WHITE << "2. They will receive your swap request\n";
        cout << "  " << WHITE << "3. If they accept, rooms will be exchanged automatically\n";
        cout << "  " << WHITE << "4. You can check status in 'View Swap Requests'\n"
             << RESET;

        printSeparator();

        // Input section with option to go back
        cout << YELLOW << "\n  Enter Student ID you want to swap with (or type 'BACK' to cancel): " << RESET;
        string targetId;
        getline(cin, targetId);

        // Check if user wants to go back
        if (targetId == "BACK" || targetId == "back" || targetId == "Back" || targetId == "b" || targetId == "B")
        {
            clearScreen();
            printInfo("Room swap request cancelled.");
            pauseAndClear();
            return;
        }

        // Validate input
        if (targetId.empty())
        {
            clearScreen();
            printError("Student ID cannot be empty!");
            cout << YELLOW << "\n  Press Enter to try again... " << RESET;
            cin.get();
            continue;
        }

        if (targetId == sid)
        {
            clearScreen();
            cout << RED << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                      ❌ INVALID REQUEST                       ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;
            cout << "\n"
                 << YELLOW << "  You cannot swap with yourself! That wouldn't make sense. 😄\n"
                 << RESET;

            cout << "\n"
                 << CYAN << "  Press Enter to try again... " << RESET;
            cin.get();
            continue;
        }

        // Check if target student exists
        Student *target = findStudentById(targetId);
        if (!target)
        {
            clearScreen();
            cout << RED << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                    ❌ STUDENT NOT FOUND                       ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;
            cout << "\n"
                 << YELLOW << "  Student ID '" << targetId << "' not found in the system.\n"
                 << RESET;
            cout << "\n"
                 << CYAN << "  Please check the ID and try again.\n"
                 << RESET;

            // Give options to try again or go back
            printSeparator();
            cout << YELLOW << "\n  What would you like to do?\n"
                 << RESET;
            cout << CYAN << "  1. Try again with different ID\n";
            cout << "  2. Go back to Student Menu\n";
            cout << YELLOW << "\n  Enter your choice (1-2): " << RESET;

            string choice;
            getline(cin, choice);

            if (choice == "2")
            {
                clearScreen();
                printInfo("Returning to Student Menu...");
                pauseAndClear();
                return;
            }
            // If choice is "1" or anything else, continue the loop (try again)
            continue;
        }

        // Check if target has a room
        if (target->getRoom().empty())
        {
            clearScreen();
            cout << RED << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                    ❌ CANNOT SWAP                             ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;
            cout << "\n"
                 << YELLOW << "  " << target->getName() << " doesn't have a room assigned yet.\n"
                 << RESET;
            cout << "\n"
                 << CYAN << "  They need to get a room assignment first before swapping.\n"
                 << RESET;

            // Give options to try again or go back
            printSeparator();
            cout << YELLOW << "\n  What would you like to do?\n"
                 << RESET;
            cout << CYAN << "  1. Try again with different student\n";
            cout << "  2. Go back to Student Menu\n";
            cout << YELLOW << "\n  Enter your choice (1-2): " << RESET;

            string choice;
            getline(cin, choice);

            if (choice == "2")
            {
                clearScreen();
                printInfo("Returning to Student Menu...");
                pauseAndClear();
                return;
            }
            continue;
        }

        // Check for existing swap requests
        for (const auto &req : swapRequests)
        {
            if ((req.fromStudentId == sid && req.toStudentId == targetId && req.status == "pending") ||
                (req.fromStudentId == targetId && req.toStudentId == sid && req.status == "pending"))
            {
                clearScreen();
                cout << YELLOW << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                   ⚠️  REQUEST EXISTS                         ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;
                cout << "\n"
                     << YELLOW << "  A swap request already exists between you and " << target->getName() << "!\n"
                     << RESET;
                cout << "\n"
                     << CYAN << "  Request ID: " << req.requestId << " | Status: " << req.status << RESET << "\n";
                cout << "\n"
                     << CYAN << "  Check 'View Swap Requests' for current status.\n"
                     << RESET;

                // Give options
                printSeparator();
                cout << YELLOW << "\n  What would you like to do?\n"
                     << RESET;
                cout << CYAN << "  1. Try again with different student\n";
                cout << "  2. View Swap Requests\n";
                cout << "  3. Go back to Student Menu\n";
                cout << YELLOW << "\n  Enter your choice (1-3): " << RESET;

                string choice;
                getline(cin, choice);

                if (choice == "2")
                {
                    viewSwapRequests(sid);
                    continue;
                }
                else if (choice == "3")
                {
                    clearScreen();
                    printInfo("Returning to Student Menu...");
                    pauseAndClear();
                    return;
                }
                // If choice is "1" or anything else, continue the loop (try again)
                continue;
            }
        }

        // Show swap preview
        clearScreen();
        cout << CYAN << BOLD;
        cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
        cout << "║                      🔄 SWAP PREVIEW                          ║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n"
             << RESET;

        cout << "\n"
             << GREEN << BOLD << "  📋 Swap Details:\n"
             << RESET;
        cout << "  ├─ " << YELLOW << "From (You): " << RESET << s->getName() << " (" << sid << ")\n";
        cout << "  │   └─ " << BLUE << "Current Room: " << RESET << s->getRoom() << "\n";
        cout << "  ├─ " << YELLOW << "To: " << RESET << target->getName() << " (" << targetId << ")\n";
        cout << "  │   └─ " << BLUE << "Current Room: " << RESET << target->getRoom() << "\n";
        cout << "  └─ " << YELLOW << "After Swap: " << RESET << "\n";
        cout << "      ├─ " << GREEN << "You → " << target->getRoom() << RESET << "\n";
        cout << "      └─ " << GREEN << target->getName() << " → " << s->getRoom() << RESET << "\n";

        printSeparator();

        // Confirmation with cancel option
        cout << "\n"
             << YELLOW << BOLD << "  ❓ CONFIRM SWAP REQUEST\n"
             << RESET;
        cout << "  " << WHITE << "Are you sure you want to send this swap request to " << target->getName() << "?\n"
             << RESET;

        cout << "\n"
             << CYAN << "  Enter 'YES' to confirm, 'NO' to cancel: " << RESET;
        string confirm;
        getline(cin, confirm);

        if (confirm == "YES" || confirm == "yes" || confirm == "Yes" || confirm == "Y" || confirm == "y")
        {
            showLoadingAnimation("Sending swap request", 2);

            string requestId = "SW" + to_string(swapRequests.size() + 1);
            swapRequests.emplace_back(requestId, sid, targetId, "pending");

            clearScreen();
            cout << GREEN << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                    ✅ REQUEST SENT                           ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            cout << "\n"
                 << CYAN << BOLD << "  📨 Request Details:\n"
                 << RESET;
            cout << "  ├─ " << YELLOW << "Request ID: " << RESET << requestId << "\n";
            cout << "  ├─ " << YELLOW << "Sent To: " << RESET << target->getName() << " (" << targetId << ")\n";
            cout << "  ├─ " << YELLOW << "Your Room: " << RESET << s->getRoom() << "\n";
            cout << "  ├─ " << YELLOW << "Their Room: " << RESET << target->getRoom() << "\n";
            cout << "  └─ " << YELLOW << "Status: " << RESET << "⏳ Pending Approval\n";

            printSeparator();
            cout << "\n"
                 << YELLOW << "  📢 " << target->getName() << " will be notified of your request.\n"
                 << RESET;
            cout << "  " << CYAN << "You can check the status in 'View Swap Requests'.\n"
                 << RESET;

            printBox("Swap request sent successfully! 📬");
        }
        else
        {
            clearScreen();
            printInfo("Swap request cancelled.");
            printBox("No changes were made. 🔄");
        }

        pauseAndClear();
        return; // Always return after processing (success or cancel)
    }
}

void HallSystem::viewSwapRequests(const string &sid)
{
    while (true)
    {
        clearScreen();

        // Beautiful header design
        cout << MAGENTA << BOLD;
        cout << "\n╔════════════════════════════════════════════════════════════════╗\n";

        string title = "🔄 MY SWAP REQUESTS";
        int titlePadding = (64 - title.length()) / 2;
        cout << "║" << string(titlePadding, ' ') << title
             << string(64 - titlePadding - title.length(), ' ') << "  ║\n";

        cout << "╚════════════════════════════════════════════════════════════════╝\n"
             << RESET;

        Student *currentStudent = findStudentById(sid);
        if (!currentStudent)
        {
            printError("Student not found!");
            pauseAndClear();
            return;
        }

        // Get current student's room info
        string currentRoom = currentStudent->getRoom();
        cout << "\n"
             << CYAN << BOLD << "  🏠 Your Current Room: " << RESET
             << (currentRoom.empty() ? RED "Not assigned" : GREEN + currentRoom) << RESET << "\n";

        printSeparator();

        vector<SwapRequest *> mySentRequests;
        vector<SwapRequest *> receivedRequests;
        vector<SwapRequest *> completedRequests;

        // Categorize requests
        for (auto &req : swapRequests)
        {
            if (req.fromStudentId == sid)
                mySentRequests.push_back(&req);
            else if (req.toStudentId == sid)
            {
                if (req.status == "pending")
                    receivedRequests.push_back(&req);
                else
                    completedRequests.push_back(&req);
            }
        }

        bool hasAnyRequests = !mySentRequests.empty() || !receivedRequests.empty() || !completedRequests.empty();

        if (!hasAnyRequests)
        {
            // No requests found - show beautiful empty state
            cout << "\n\n";
            cout << CYAN << BOLD;
            cout << "  ╔═══════════════════════════════════════════════════════╗\n";
            cout << "  ║                   📭 NO REQUESTS FOUND                ║\n";
            cout << "  ╚═══════════════════════════════════════════════════════╝\n"
                 << RESET;

            cout << "\n"
                 << YELLOW << "  ℹ️  You don't have any swap requests yet.\n"
                 << RESET;
            cout << "\n"
                 << CYAN << "  Here's what you can do:\n"
                 << RESET;
            cout << "  ├─ " << GREEN << "Send a swap request to another student\n"
                 << RESET;
            cout << "  ├─ " << BLUE << "Wait for other students to send you requests\n"
                 << RESET;
            cout << "  └─ " << YELLOW << "Check back later for new requests\n"
                 << RESET;

            printSeparator();

            cout << YELLOW << "\n  What would you like to do?\n"
                 << RESET;
            cout << CYAN << "  1. Send a new swap request\n";
            cout << "  2. Go back to Student Menu\n";
            cout << YELLOW << "\n  Enter your choice (1-2): " << RESET;

            string choice;
            getline(cin, choice);

            if (choice == "1")
            {
                requestRoomSwap(sid);
                continue; // Come back to this menu after sending request
            }
            else
            {
                return; // Go back to student menu
            }
        }

        // Display sent requests
        if (!mySentRequests.empty())
        {
            cout << "\n"
                 << CYAN << BOLD << "  📤 REQUESTS YOU SENT:\n"
                 << RESET;
            cout << "  " << YELLOW << "These are swap requests you've sent to other students:\n"
                 << RESET;

            for (size_t i = 0; i < mySentRequests.size(); i++)
            {
                const auto *req = mySentRequests[i];
                Student *target = findStudentById(req->toStudentId);
                Student *from = findStudentById(req->fromStudentId);

                cout << "\n"
                     << MAGENTA << "  ┌─ " << "Request #" << (i + 1) << " " << string(50, '-') << "┐\n"
                     << RESET;

                cout << "  │ " << YELLOW << "Request ID: " << RESET << req->requestId << "\n";
                cout << "  │ " << YELLOW << "To: " << RESET << (target ? target->getName() : "Unknown")
                     << " (" << req->toStudentId << ")\n";
                cout << "  │ " << YELLOW << "Their Room: " << RESET << (target ? target->getRoom() : "N/A") << "\n";

                // Status with color coding
                cout << "  │ " << YELLOW << "Status: " << RESET;
                if (req->status == "pending")
                    cout << YELLOW << "⏳ Pending (Waiting for response)" << RESET << "\n";
                else if (req->status == "accepted")
                    cout << GREEN << "✅ Accepted (Swap completed)" << RESET << "\n";
                else if (req->status == "rejected")
                    cout << RED << "❌ Rejected" << RESET << "\n";
                else
                    cout << req->status << "\n";

                // Show room exchange details
                if (from && target)
                {
                    cout << "  │ " << YELLOW << "Swap: " << RESET
                         << from->getRoom() << " ↔ " << target->getRoom() << "\n";
                }

                cout << MAGENTA << "  └" << string(58, '-') << "┘\n"
                     << RESET;
            }
        }

        // Display received requests
        if (!receivedRequests.empty())
        {
            cout << "\n"
                 << CYAN << BOLD << "  📥 REQUESTS RECEIVED:\n"
                 << RESET;
            cout << "  " << YELLOW << "These students want to swap rooms with you:\n"
                 << RESET;

            for (size_t i = 0; i < receivedRequests.size(); i++)
            {
                const auto *req = receivedRequests[i];
                Student *from = findStudentById(req->fromStudentId);

                cout << "\n"
                     << GREEN << "  ┌─ " << "Request #" << (i + 1) << " " << string(50, '-') << "┐\n"
                     << RESET;

                cout << "  │ " << YELLOW << "Request ID: " << RESET << req->requestId << "\n";
                cout << "  │ " << YELLOW << "From: " << RESET << (from ? from->getName() : "Unknown")
                     << " (" << req->fromStudentId << ")\n";
                cout << "  │ " << YELLOW << "Their Room: " << RESET << (from ? from->getRoom() : "N/A") << "\n";
                cout << "  │ " << YELLOW << "Status: " << YELLOW << "⏳ Pending - Your response needed!\n"
                     << RESET;

                // Show room exchange details
                if (from && currentStudent)
                {
                    cout << "  │ " << YELLOW << "Swap: " << RESET
                         << from->getRoom() << " ↔ " << currentStudent->getRoom() << "\n";
                }

                cout << GREEN << "  └" << string(58, '-') << "┘\n"
                     << RESET;
            }

            // Action section for received requests
            printSeparator();
            cout << YELLOW << "\n  🎯 ACTIONS AVAILABLE:\n"
                 << RESET;
            cout << "  " << CYAN << "You can respond to pending requests below.\n"
                 << RESET;

            cout << YELLOW << "\n  Would you like to respond to any request? (y/n): " << RESET;
            char respond;
            cin >> respond;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (respond == 'y' || respond == 'Y')
            {
                cout << YELLOW << "\n  Enter Request ID to respond: " << RESET;
                string reqId;
                getline(cin, reqId);

                // Find the request
                SwapRequest *targetRequest = nullptr;
                for (auto &req : swapRequests)
                {
                    if (req.requestId == reqId && req.toStudentId == sid && req.status == "pending")
                    {
                        targetRequest = &req;
                        break;
                    }
                }

                if (targetRequest)
                {
                    Student *fromStudent = findStudentById(targetRequest->fromStudentId);

                    clearScreen();
                    cout << CYAN << BOLD;
                    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                    cout << "║                   🤔 RESPOND TO REQUEST                     ║\n";
                    cout << "╚════════════════════════════════════════════════════════════════╝\n"
                         << RESET;

                    cout << "\n"
                         << GREEN << BOLD << "  📋 Request Details:\n"
                         << RESET;
                    cout << "  ├─ " << YELLOW << "Request ID: " << RESET << targetRequest->requestId << "\n";
                    cout << "  ├─ " << YELLOW << "From: " << RESET << (fromStudent ? fromStudent->getName() : "Unknown") << "\n";
                    cout << "  ├─ " << YELLOW << "Their Room: " << RESET << (fromStudent ? fromStudent->getRoom() : "N/A") << "\n";
                    cout << "  ├─ " << YELLOW << "Your Room: " << RESET << currentRoom << "\n";
                    cout << "  └─ " << YELLOW << "After Swap: " << RESET << "\n";
                    cout << "      ├─ " << GREEN << "You → " << (fromStudent ? fromStudent->getRoom() : "N/A") << RESET << "\n";
                    cout << "      └─ " << GREEN << (fromStudent ? fromStudent->getName() : "They") << " → " << currentRoom << RESET << "\n";

                    printSeparator();

                    cout << YELLOW << "\n  How would you like to respond?\n"
                         << RESET;
                    cout << CYAN << "  1. ✅ Accept - Swap rooms with this student\n";
                    cout << "  2. ❌ Reject - Decline the swap request\n";
                    cout << "  3. ↩️  Cancel - Go back without responding\n";
                    cout << YELLOW << "\n  Enter your choice (1-3): " << RESET;

                    string choice;
                    getline(cin, choice);

                    if (choice == "1")
                    {
                        processSwapRequest(sid, reqId, true);
                        continue; // Refresh the view after processing
                    }
                    else if (choice == "2")
                    {
                        processSwapRequest(sid, reqId, false);
                        continue; // Refresh the view after processing
                    }
                    // If choice is 3 or anything else, just continue (show requests again)
                }
                else
                {
                    clearScreen();
                    printError("Request not found or already processed!");
                    pauseAndClear();
                }
            }
        }

        // Display completed requests (history)
        if (!completedRequests.empty())
        {
            cout << "\n"
                 << CYAN << BOLD << "  📜 REQUEST HISTORY:\n"
                 << RESET;
            cout << "  " << YELLOW << "Previously completed or rejected requests:\n"
                 << RESET;

            for (size_t i = 0; i < completedRequests.size(); i++)
            {
                const auto *req = completedRequests[i];
                Student *otherStudent = findStudentById(
                    req->fromStudentId == sid ? req->toStudentId : req->fromStudentId);

                string requestType = (req->fromStudentId == sid) ? "You sent to" : "Received from";
                string otherId = (req->fromStudentId == sid) ? req->toStudentId : req->fromStudentId;

                cout << "\n"
                     << BLUE << "  +-- " << "History #" << (i + 1) << " " << string(48, '-') << "+\n"
                     << RESET;

                cout << "  │ " << YELLOW << "Request ID: " << RESET << req->requestId << "\n";
                cout << "  │ " << YELLOW << requestType << ": " << RESET
                     << (otherStudent ? otherStudent->getName() : "Unknown") << " (" << otherId << ")\n";

                // Status with color coding
                cout << "  │ " << YELLOW << "Status: " << RESET;
                if (req->status == "accepted")
                    cout << GREEN << "✅ Completed" << RESET << "\n";
                else if (req->status == "rejected")
                {
                    if (req->fromStudentId == sid)
                        cout << RED << "❌ Rejected by them" << RESET << "\n";
                    else
                        cout << RED << "❌ You rejected" << RESET << "\n";
                }

                cout << BLUE << "  +" << string(58, '-') << "+\n"
                     << RESET;
            }
        }

        printSeparator();

        // Navigation options
        cout << YELLOW << "\n  🎮 NAVIGATION OPTIONS:\n"
             << RESET;
        cout << CYAN << "  1. Send a new swap request\n";
        if (!receivedRequests.empty())
            cout << "  2. Respond to pending requests\n";
        cout << "  3. Refresh this page\n";
        cout << "  4. Back to Student Menu\n";
        cout << YELLOW << "\n  Enter your choice (1-4): " << RESET;

        string choice;
        getline(cin, choice);

        if (choice == "1")
        {
            requestRoomSwap(sid);
        }
        else if (choice == "2" && !receivedRequests.empty())
        {
            // This will naturally happen in the next loop iteration
            continue;
        }
        else if (choice == "3")
        {
            continue; // Refresh
        }
        else if (choice == "4")
        {
            clearScreen();
            printSuccess("Returning to Student Menu...");
            pauseAndClear();
            return;
        }
        else
        {
            printError("Invalid choice!");
            pauseAndClear();
        }
    }
}

void HallSystem::processSwapRequest(const string &sid, const string &requestId, bool accept)
{
    for (auto &req : swapRequests)
    {
        if (req.requestId == requestId && req.toStudentId == sid && req.status == "pending")
        {
            if (accept)
            {
                Student *student1 = findStudentById(req.fromStudentId);
                Student *student2 = findStudentById(req.toStudentId);

                if (!student1 || !student2)
                {
                    cout << "\nError: Student not found!\n";
                    pauseAndClear();
                    return;
                }

                string room1 = student1->getRoom();
                string room2 = student2->getRoom();

                if (room1.empty() || room2.empty())
                {
                    cout << "\nError: One or both students don't have rooms!\n";
                    pauseAndClear();
                    return;
                }

                Room *r1 = findRoomByNo(room1);
                Room *r2 = findRoomByNo(room2);

                if (!r1 || !r2)
                {
                    cout << "\nError: Room not found!\n";
                    pauseAndClear();
                    return;
                }

                r1->occupants.erase(remove(r1->occupants.begin(), r1->occupants.end(), req.fromStudentId),
                                    r1->occupants.end());
                r2->occupants.erase(remove(r2->occupants.begin(), r2->occupants.end(), req.toStudentId),
                                    r2->occupants.end());

                r1->occupants.push_back(req.toStudentId);
                r2->occupants.push_back(req.fromStudentId);

                student1->setRoom(room2);
                student2->setRoom(room1);

                req.status = "accepted";

                cout << "\n✓ Swap completed successfully!\n";
                cout << student1->getName() << " moved to Room " << room2 << "\n";
                cout << student2->getName() << " moved to Room " << room1 << "\n";
            }
            else
            {
                req.status = "rejected";
                cout << "\n✗ Swap request rejected.\n";
            }

            pauseAndClear();
            return;
        }
    }

    cout << "\nRequest not found or already processed!\n";
    pauseAndClear();
}

// ------------------ Meal Management Functions ------------------

void HallSystem::manageMealPlan(const string &sid)
{
    printHeader("MEAL PLAN MANAGEMENT");

    MealPlan *mp = findMealPlanByStudentId(sid);
    if (!mp || !(*mp))
    { // Using overloaded bool operator - checks if meal plan is active
        printInfo("No active meal plan found.");
        return;
    }

    cout << "\n"
         << CYAN << "  Current Meal Plan:\n"
         << RESET;
    cout << "  ├─ Lunch : " << (mp->hasLunch ? GREEN "✓ Subscribed" : RED "✗ Not Subscribed") << RESET << "\n";
    cout << "  └─ Dinner: " << (mp->hasDinner ? GREEN "✓ Subscribed" : RED "✗ Not Subscribed") << RESET << "\n";
    cout << "\n"
         << YELLOW << "  Daily Rate: " << RESET << DAILY_MEAL_RATE << " Tk (Lunch + Dinner)\n\n";

    printSeparator();
    printMenuOption(1, "Subscribe to Lunch Only");
    printMenuOption(2, "Subscribe to Dinner Only");
    printMenuOption(3, "Subscribe to Both (Lunch & Dinner)");
    printMenuOption(4, "Unsubscribe from All");
    printMenuOption(5, "Back");
    printSeparator();

    cout << YELLOW << "\n  Choice: " << RESET;

    int choice;
    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pauseAndClear();
        return;
    }

    showLoadingAnimation("Updating meal plan", 1);

    switch (choice)
    {
    case 1:
        mp->hasLunch = true;
        mp->hasDinner = false;
        printSuccess("Subscribed to Lunch only!");
        break;
    case 2:
        mp->hasLunch = false;
        mp->hasDinner = true;
        printSuccess("Subscribed to Dinner only!");
        break;
    case 3:
        mp->hasLunch = true;
        mp->hasDinner = true;
        printSuccess("Subscribed to Both Lunch & Dinner!");
        break;
    case 4:
        mp->hasLunch = false;
        mp->hasDinner = false;
        printSuccess("Unsubscribed from all meals!");
        break;
    case 5:
        return;
    default:
        printError("Invalid choice!");
        pauseAndClear();
        return;
    }

    pauseAndClear();
}

void HallSystem::markMealOff(const string &sid)
{
    clearScreen();

    MealPlan *mp = findMealPlanByStudentId(sid);
    if (!mp)
    {
        printError("No meal plan found. Please subscribe first.");
        pauseAndClear();
        return;
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    int selectedYear = currentYear;
    int selectedMonth = currentMonth;

    while (true)
    {
        clearScreen();

        // ───────────── Header ─────────────
        cout << MAGENTA << BOLD;
        cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
        string title = "🍽️  MARK MEAL OFF DAYS";
        int pad = (64 - title.length()) / 2;
        cout << "║" << string(pad, ' ') << title << string(64 - pad - title.length(), ' ') << "      ║\n";
        cout << "╚════════════════════════════════════════════════════════════════╝\n"
             << RESET;

        // ───────────── Month Header ─────────────
        string monthNames[] = {"", "January", "February", "March", "April", "May", "June",
                               "July", "August", "September", "October", "November", "December"};

        cout << "\n"
             << CYAN << BOLD << "  📅 " << monthNames[selectedMonth] << " " << selectedYear << RESET << "\n";

        // ───────────── Meal Stats ─────────────
        int offDaysCount = 0;
        for (const auto &date : mp->offDates)
        {
            if (date.substr(0, 7) == (to_string(selectedYear) + "-" + (selectedMonth < 10 ? "0" : "") + to_string(selectedMonth)))
                offDaysCount++;
        }

        int daysInMonth = getDaysInMonth(to_string(selectedYear) + "-" + to_string(selectedMonth));
        cout << "\n"
             << YELLOW << BOLD << "  📊 Meal Summary:\n"
             << RESET;
        cout << "  ├─ " << RED << "Off Days: " << RESET << offDaysCount << "\n";
        cout << "  ├─ " << GREEN << "On Days : " << RESET << (daysInMonth - offDaysCount) << "\n";
        cout << "  └─ " << CYAN << "Total Days: " << RESET << daysInMonth << "\n";

        printSeparator();

        // ───────────── Calendar ─────────────
        cout << "\n"
             << CYAN << BOLD << "  🗓️  Calendar:\n"
             << RESET;

        // Weekday headers
        cout << "   ";
        string weekDays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        for (int i = 0; i < 7; i++)
        {
            if (i == 0)
                cout << RED;
            else if (i == 6)
                cout << BLUE;
            else
                cout << YELLOW;
            cout << " " << setw(3) << left << weekDays[i] << RESET;
        }
        cout << "\n";

        // Top border
        cout << "   " << CYAN << " ┌────────────────────────────┐\n"
             << RESET;

        // Calculate first day
        tm firstDay = {0};
        firstDay.tm_year = selectedYear - 1900;
        firstDay.tm_mon = selectedMonth - 1;
        firstDay.tm_mday = 1;
        mktime(&firstDay);

        int startDay = firstDay.tm_wday;
        int daysInCurrentMonth = getDaysInMonth(to_string(selectedYear) + "-" + to_string(selectedMonth));

        // Print weeks
        for (int week = 0; week < 6; week++)
        {
            cout << "   " << CYAN << " │" << RESET;

            for (int dayOfWeek = 0; dayOfWeek < 7; dayOfWeek++)
            {
                int dayNumber = week * 7 + dayOfWeek - startDay + 1;

                if (dayNumber < 1 || dayNumber > daysInCurrentMonth)
                {
                    cout << "    "; // Empty cell
                }
                else
                {
                    char buffer[12];
                    snprintf(buffer, 12, "%04d-%02d-%02d", selectedYear, selectedMonth, dayNumber);
                    string dateStr = buffer;

                    bool isOff = (mp->offDates.find(dateStr) != mp->offDates.end());
                    bool isToday = (dayNumber == currentDay && selectedMonth == currentMonth && selectedYear == currentYear);
                    bool isWeekend = (dayOfWeek == 0 || dayOfWeek == 6);

                    if (isOff)
                        cout << BG_RED << BLACK << "[" << setw(2) << dayNumber << "]" << RESET;
                    else if (isToday)
                        cout << BG_GREEN << BLACK << "<" << setw(2) << dayNumber << ">" << RESET;
                    else if (isWeekend)
                        cout << BLUE << " " << setw(2) << dayNumber << " " << RESET;
                    else
                        cout << WHITE << " " << setw(2) << dayNumber << " " << RESET;
                }
            }

            cout << CYAN << "│" << RESET << "\n";

            // Stop if we've printed all days
            if (week * 7 + 7 - startDay >= daysInCurrentMonth)
                break;
        }

        // Bottom border
        cout << "   " << CYAN << " └────────────────────────────┘\n"
             << RESET;

        // ───────────── Legend ─────────────
        cout << "\n"
             << CYAN << BOLD << "  📖 Legend:\n"
             << RESET;
        cout << "  " << BG_RED << BLACK << " [DD] " << RESET << " = " << RED << "Meal Off" << RESET << "   ";
        cout << BG_GREEN << BLACK << " DD " << RESET << " = " << GREEN << "Today" << RESET << "   ";
        cout << BLUE << " DD " << RESET << " = " << BLUE << "Weekend" << RESET << "   ";
        cout << WHITE << " DD " << RESET << " = " << WHITE << "Normal" << RESET << "\n";

        printSeparator();

        // ───────────── Menu ─────────────
        cout << "\n"
             << CYAN << BOLD << "  🎯 Actions:\n"
             << RESET;
        printMenuOption(1, "📝 Mark/Unmark a Date", false);
        printMenuOption(2, "⬅️  Previous Month", false);
        printMenuOption(3, "➡️  Next Month", false);
        printMenuOption(4, "🔍 Go to Specific Month", false);
        printMenuOption(5, "🗑️  Clear All Off Days", false);
        printMenuOption(6, "🔙 Back to Dashboard", true);

        printSeparator();

        cout << YELLOW << "\n  🎮 Enter your choice (1-6): " << RESET;

        int choice;
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            clearScreen();
            printError("Invalid input! Please enter a number between 1-6.");
            pauseAndClear();
            continue;
        }

        switch (choice)
        {
        case 1: // Mark/Unmark a Date
        {
            int daysInCurrentMonth = getDaysInMonth(to_string(selectedYear) + "-" + to_string(selectedMonth));
            cout << YELLOW << "\n  Enter day (1-" << daysInCurrentMonth << "): " << RESET;
            int day;
            if (!(cin >> day))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                printError("Invalid day!");
                pauseAndClear();
                break;
            }

            if (day < 1 || day > daysInCurrentMonth)
            {
                printError("Invalid day! Please enter a valid day.");
                pauseAndClear();
                break;
            }

            char buffer[12];
            snprintf(buffer, 12, "%04d-%02d-%02d", selectedYear, selectedMonth, day);
            string dateStr = buffer;

            showLoadingAnimation("Updating meal status", 1);

            if (mp->offDates.find(dateStr) != mp->offDates.end())
            {
                mp->offDates.erase(dateStr);
                clearScreen();
                printSuccess("Meal turned back ON for " + monthNames[selectedMonth] + " " + to_string(day) + "!");
                printBox("You'll be charged for this day 💰");
            }
            else
            {
                mp->offDates.insert(dateStr);
                clearScreen();
                printSuccess("Meal marked OFF for " + monthNames[selectedMonth] + " " + to_string(day) + "!");
                printBox("You won't be charged for this day 🎉");
            }
            pauseAndClear();
            break;
        }

        case 2: // Previous month
            selectedMonth--;
            if (selectedMonth < 1)
            {
                selectedMonth = 12;
                selectedYear--;
            }
            break;

        case 3: // Next month
            selectedMonth++;
            if (selectedMonth > 12)
            {
                selectedMonth = 1;
                selectedYear++;
            }
            break;

        case 4: // Go to specific month
        {
            cout << YELLOW << "\n  Enter Year (e.g., " << currentYear << "): " << RESET;
            if (!(cin >> selectedYear))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                printError("Invalid year!");
                pauseAndClear();
                break;
            }

            cout << YELLOW << "  Enter Month (1-12): " << RESET;
            if (!(cin >> selectedMonth))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                printError("Invalid month!");
                pauseAndClear();
                break;
            }

            if (selectedMonth < 1 || selectedMonth > 12)
            {
                printError("Invalid month! Using current month.");
                selectedMonth = currentMonth;
                selectedYear = currentYear;
                pauseAndClear();
            }
            break;
        }

        case 5: // Clear all off dates this month
        {
            string monthYearStr = to_string(selectedYear) + "-" +
                                  (selectedMonth < 10 ? "0" : "") + to_string(selectedMonth);

            int currentOffCount = 0;
            for (const auto &date : mp->offDates)
            {
                if (date.substr(0, 7) == monthYearStr)
                    currentOffCount++;
            }

            if (currentOffCount == 0)
            {
                clearScreen();
                printInfo("No meal off days to clear for " + monthNames[selectedMonth] + "!");
                pauseAndClear();
                break;
            }

            clearScreen();
            cout << RED << BOLD;
            cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
            cout << "║                    🗑️  CLEAR CONFIRMATION                     ║\n";
            cout << "╚════════════════════════════════════════════════════════════════╝\n"
                 << RESET;

            cout << "\n"
                 << YELLOW << "  You are about to clear " << currentOffCount
                 << " meal off days for " << monthNames[selectedMonth] << " " << selectedYear << ".\n"
                 << RESET;

            cout << "\n"
                 << RED << "  This action cannot be undone!\n"
                 << RESET;

            cout << YELLOW << "\n  Type 'CLEAR' to confirm: " << RESET;
            string confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, confirm);

            if (confirm == "CLEAR")
            {
                showLoadingAnimation("Clearing meal off days", 2);

                auto it = mp->offDates.begin();
                while (it != mp->offDates.end())
                {
                    if (it->substr(0, 7) == monthYearStr)
                        it = mp->offDates.erase(it);
                    else
                        ++it;
                }

                clearScreen();
                printSuccess("All meal off days cleared for " + monthNames[selectedMonth] + "!");
                printBox("All days are now set to MEAL ON 🍽️");
            }
            else
            {
                clearScreen();
                printInfo("Clear operation cancelled.");
            }
            pauseAndClear();
            break;
        }

        case 6: // Back to dashboard
            clearScreen();
            printSuccess("Returning to Dashboard...");
            pauseAndClear();
            return;

        default:
            clearScreen();
            printError("Invalid choice! Please select 1-6.");
            pauseAndClear();
            break;
        }
    }
}
// ==================== VIEW TODAY'S MENU ====================
void HallSystem::viewTodayMenu(const string &sid)
{
    printHeader("TODAY'S MENU");

    string today = getCurrentDate();
    cout << "\n"
         << CYAN << "  📅 Date: " << RESET << today << "\n\n";

    DailyMenu *menu = findMenuByDate(today);

    if (menu)
    {
        cout << GREEN << "  🍽️  Lunch Menu:\n"
             << RESET;
        cout << "     " << menu->lunchMenu << "\n\n";

        cout << GREEN << "  🍽️  Dinner Menu:\n"
             << RESET;
        cout << "     " << menu->dinnerMenu << "\n";
    }
    else
    {
        printWarning("No menu available for today.");
        printInfo("Please contact administration to update the menu.");
    }

    pauseAndClear();
}

// ==================== ENHANCED MEAL PLAN VIEW ====================

void HallSystem::viewMealPlan(const string &sid)
{
    printHeader("MY MEAL PLAN");

    MealPlan *mp = findMealPlanByStudentId(sid);
    if (!mp)
    {
        printWarning("No meal plan found.");
        pauseAndClear();
        return;
    }

    cout << "\n"
         << CYAN << BOLD << "  📊 Subscription Status:\n"
         << RESET;
    cout << "  ├─ Lunch : " << (mp->hasLunch ? GREEN "✓ Active" : RED "✗ Inactive") << RESET << "\n";
    cout << "  └─ Dinner: " << (mp->hasDinner ? GREEN "✓ Active" : RED "✗ Inactive") << RESET << "\n";

    cout << "\n"
         << YELLOW << "  💰 Daily Rate: " << RESET << DAILY_MEAL_RATE << " Tk\n";

    if (!mp->offDates.empty())
    {
        cout << "\n"
             << CYAN << BOLD << "  📅 Meal Off Dates:\n"
             << RESET;
        int count = 0;
        for (const auto &date : mp->offDates)
        {
            if (count % 3 == 0)
                cout << "  ";
            cout << RED << "• " << date << RESET << "  ";
            count++;
            if (count % 3 == 0)
                cout << "\n";
        }
        if (count % 3 != 0)
            cout << "\n";

        cout << "\n"
             << YELLOW << "  Total Off Days: " << RESET << mp->offDates.size() << "\n";
    }
    else
    {
        cout << "\n"
             << GREEN << "  ✓ No meal off dates.\n"
             << RESET;
    }

    pauseAndClear();
}

// ==================== ENHANCED PAYMENT SLIP VIEW ====================

void HallSystem::studentViewPaymentSlips(const string &sid)
{
    printHeader("MY PAYMENT SLIPS");

    vector<Payment *> myPayments;
    for (auto &p : payments)
    {
        if (p.studentId == sid)
            myPayments.push_back(&p);
    }

    if (myPayments.empty())
    {
        printWarning("No payment slips found.");
        printInfo("Administration will generate bills at the end of each month.");
        pauseAndClear();
        return;
    }

    cout << "\n";
    vector<string> headers = {"Payment ID", "Month", "Amount", "Status", "Date"};
    vector<int> widths = {13, 12, 12, 12, 15};

    printTableHeader(headers, widths);

    double totalPending = 0;
    double totalPaid = 0;

    for (const auto *p : myPayments)
    {
        vector<string> row = {
            p->paymentId,
            p->monthYear,
            to_string((int)p->amount) + " Tk",
            p->status,
            p->date == "-" ? "N/A" : p->date};

        string color = (p->hasStatus("paid")) ? "green" : "red";
        printTableRow(row, widths, color);

        if (p->status == "pending")
            totalPending += p->amount;
        else
            totalPaid += p->amount;
    }

    printSeparator();

    if (totalPending > 0)
    {
        cout << "\n"
             << RED << BOLD << "  ⚠️  Total Due: " << fixed << setprecision(2)
             << totalPending << " Tk" << RESET << "\n";
        printWarning("Please make payment as soon as possible!");
    }
    else
    {
        cout << "\n"
             << GREEN << BOLD << "  ✓ All payments completed! ("
             << totalPaid << " Tk paid)" << RESET << "\n";
    }

    pauseAndClear();
}

// ==================== ENHANCED MAKE PAYMENT ====================

void HallSystem::makePayment(const string &sid)
{
    printHeader("MAKE PAYMENT");

    vector<Payment *> pendingPayments;
    for (auto &p : payments)
    {
        if (p.studentId == sid && p.status == "pending")
            pendingPayments.push_back(&p);
    }

    if (pendingPayments.empty())
    {
        printSuccess("No pending payments!");
        printBox("All bills are paid ✓");
        pauseAndClear();
        return;
    }

    cout << "\n"
         << RED << BOLD << "  Pending Payments:\n"
         << RESET;
    printSeparator();

    vector<string> headers = {"#", "Payment ID", "Month", "Amount"};
    vector<int> widths = {5, 13, 12, 15};
    printTableHeader(headers, widths);

    for (size_t i = 0; i < pendingPayments.size(); i++)
    {
        vector<string> row = {
            to_string(i + 1),
            pendingPayments[i]->paymentId,
            pendingPayments[i]->monthYear,
            to_string((int)pendingPayments[i]->amount) + " Tk"};
        printTableRow(row, widths, "yellow");
    }

    printSeparator();

    cout << YELLOW << "\n  Enter Payment ID to pay: " << RESET;
    string paymentId;
    cin >> ws;
    getline(cin, paymentId);

    Payment *selectedPayment = nullptr;
    for (auto &p : payments)
    {
        if (p == paymentId && p.hasStudentId(sid) && p.hasStatus("pending"))
        {
            selectedPayment = &p;
            break;
        }
    }

    if (!selectedPayment)
    {
        printError("Invalid Payment ID or already paid!");
        pauseAndClear();
        return;
    }

    // Display payment slip
    clearScreen();
    cout << CYAN << BOLD;
    cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    cout << "║                     💰 PAYMENT SLIP 💰                        ║\n";
    cout << "╠═══════════════════════════════════════════════════════════════╣\n";
    cout << RESET;

    Student *s = findStudentById(sid);
    string name = s ? s->getName() : "Unknown";

    cout << CYAN << "║  " << RESET << "Payment ID    : " << YELLOW << selectedPayment->paymentId
         << string(41 - selectedPayment->paymentId.length(), ' ') << CYAN << "    ║\n";
    cout << "║  " << RESET << "Student       : " << WHITE << name
         << string(41 - name.length(), ' ') << CYAN << "    ║\n";
    cout << "║  " << RESET << "Month         : " << WHITE << selectedPayment->monthYear
         << string(41 - selectedPayment->monthYear.length(), ' ') << CYAN << "    ║\n";
    cout << "║  " << RESET << "Amount        : " << GREEN << BOLD << fixed << setprecision(2)
         << selectedPayment->amount << " Tk" << RESET
         << string(37 - to_string((int)selectedPayment->amount).length(), ' ') << CYAN << "  ║\n";
    cout << "║  " << RESET << "Status        : " << RED << "PENDING"
         << string(37, ' ') << CYAN << " ║\n";
    cout << "╚═══════════════════════════════════════════════════════════════╝\n"
         << RESET;

    printWarning("Confirm payment? (y/n): ");
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        showLoadingAnimation("Processing payment", 2);

        selectedPayment->status = "paid";
        selectedPayment->date = getCurrentDate();

        clearScreen();
        printSuccess("Payment completed successfully!");

        // Show receipt
        cout << GREEN << BOLD;
        cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
        cout << "║                     ✓ PAYMENT RECEIPT ✓                       ║\n";
        cout << "╠═══════════════════════════════════════════════════════════════╣\n";
        cout << RESET;

        cout << GREEN << "║  " << RESET << "Payment ID    : " << selectedPayment->paymentId
             << string(41 - selectedPayment->paymentId.length(), ' ') << GREEN << "    ║\n";
        cout << "║  " << RESET << "Student       : " << name
             << string(41 - name.length(), ' ') << GREEN << "    ║\n";
        cout << "║  " << RESET << "Amount Paid   : " << BOLD << selectedPayment->amount << " Tk" << RESET
             << string(37 - to_string((int)selectedPayment->amount).length(), ' ') << GREEN << "  ║\n";
        cout << "║  " << RESET << "Payment Date  : " << selectedPayment->date
             << string(41 - selectedPayment->date.length(), ' ') << GREEN << "    ║\n";
        cout << "║  " << RESET << "Status        : " << GREEN << BOLD << "✓ PAID" << RESET
             << string(39, ' ') << GREEN << "║\n";
        cout << "╚═══════════════════════════════════════════════════════════════╝\n"
             << RESET;

        printBox("Thank you for your payment! 🎉");
    }
    else
    {
        printInfo("Payment cancelled.");
    }

    pauseAndClear();
}

// ==================== ENHANCED ADMINISTRATION UPDATE MENU ====================

void HallSystem::administrationUpdateMenu()
{
    printHeader("UPDATE DAILY MENU");

    cout << YELLOW << "\n  Enter date (YYYY-MM-DD) or press Enter for today: " << RESET;
    string date;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, date);

    if (date.empty())
    {
        date = getCurrentDate();
        printInfo("Using today's date: " + date);
    }

    DailyMenu *existingMenu = findMenuByDate(date);

    if (existingMenu)
    {
        cout << "\n"
             << CYAN << "  📋 Existing menu for " << date << ":\n"
             << RESET;
        cout << "  Lunch : " << WHITE << existingMenu->lunchMenu << RESET << "\n";
        cout << "  Dinner: " << WHITE << existingMenu->dinnerMenu << RESET << "\n\n";

        printWarning("Do you want to update it? (y/n): ");
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice != 'y' && choice != 'Y')
        {
            printInfo("Update cancelled.");
            pauseAndClear();
            return;
        }
    }

    cout << "\n"
         << YELLOW << "  🍽️  Enter Lunch Menu: " << RESET;
    string lunchMenu;
    getline(cin, lunchMenu);

    cout << YELLOW << "  🍽️  Enter Dinner Menu: " << RESET;
    string dinnerMenu;
    getline(cin, dinnerMenu);

    showLoadingAnimation("Updating menu", 1);

    if (existingMenu)
    {
        existingMenu->lunchMenu = lunchMenu;
        existingMenu->dinnerMenu = dinnerMenu;
        printSuccess("Menu updated successfully!");
    }
    else
    {
        menus.emplace_back(date, lunchMenu, dinnerMenu);
        printSuccess("Menu added successfully!");
    }

    cout << "\n"
         << GREEN << "  ✓ Menu for " << date << ":\n"
         << RESET;
    cout << "  🍽️  Lunch : " << WHITE << lunchMenu << RESET << "\n";
    cout << "  🍽️  Dinner: " << WHITE << dinnerMenu << RESET << "\n";

    printBox("Menu published! 📋");

    pauseAndClear();
}

// ==================== ENHANCED ADMINISTRATION VIEW PAYMENTS ====================

void HallSystem::administrationViewPayments()
{
    printHeader("PAYMENT RECORDS");

    if (payments.empty())
    {
        printWarning("No payment records found.");
        pauseAndClear();
        return;
    }

    cout << "\n";
    vector<string> headers = {"Payment ID", "Student ID", "Month", "Amount", "Status", "Date"};
    vector<int> widths = {13, 15, 12, 12, 12, 15};

    printTableHeader(headers, widths);

    double totalPaid = 0;
    double totalPending = 0;
    int paidCount = 0;
    int pendingCount = 0;

    for (const auto &p : payments)
    {
        vector<string> row = {
            p.paymentId,
            p.studentId,
            p.monthYear,
            to_string((int)p.amount) + " Tk",
            p.status,
            p.date == "-" ? "N/A" : p.date};

        string color = (p.hasStatus("paid")) ? "green" : "red";
        printTableRow(row, widths, color);

        if (p.status == "paid")
        {
            totalPaid += p.amount;
            paidCount++;
        }
        else
        {
            totalPending += p.amount;
            pendingCount++;
        }
    }

    printSeparator();

    cout << "\n"
         << CYAN << BOLD << "  📊 Summary:\n"
         << RESET;
    cout << "  ├─ " << GREEN << "Paid      : " << paidCount << " payments ("
         << fixed << setprecision(2) << totalPaid << " Tk)" << RESET << "\n";
    cout << "  ├─ " << RED << "Pending   : " << pendingCount << " payments ("
         << totalPending << " Tk)" << RESET << "\n";
    cout << "  └─ " << YELLOW << "Total     : " << payments.size() << " payments ("
         << (totalPaid + totalPending) << " Tk)" << RESET << "\n";

    if (totalPending > 0)
    {
        double collectionRate = (totalPaid / (totalPaid + totalPending)) * 100;
        cout << "\n  💹 Collection Rate: " << YELLOW << fixed << setprecision(1)
             << collectionRate << "%" << RESET << "\n";
    }

    pauseAndClear();
}

// ==================== ENHANCED ADMINISTRATION SEARCH PAYMENT ====================

void HallSystem::administrationSearchPayment()
{
    printHeader("SEARCH STUDENT PAYMENT");

    cout << YELLOW << "\n  Enter Student ID: " << RESET;
    string sid;
    cin >> ws;
    getline(cin, sid);

    Student *s = findStudentById(sid);
    if (!s)
    {
        printError("Student not found!");
        pauseAndClear();
        return;
    }

    showLoadingAnimation("Searching records", 1);
    clearScreen();

    cout << MAGENTA << BOLD;
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║              PAYMENT DETAILS FOR STUDENT                       ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n"
         << RESET;

    cout << "\n"
         << CYAN << "  Student Information:\n"
         << RESET;
    cout << "  ├─ ID         : " << YELLOW << sid << RESET << "\n";
    cout << "  ├─ Name       : " << YELLOW << s->getName() << RESET << "\n";
    cout << "  ├─ Department : " << YELLOW << s->getDept() << RESET << "\n";
    cout << "  └─ Batch      : " << YELLOW << s->getBatch() << RESET << "\n\n";

    vector<Payment *> studentPayments;
    for (auto &p : payments)
    {
        if (p.hasStudentId(sid))
            studentPayments.push_back(&p);
    }

    if (studentPayments.empty())
    {
        printWarning("No payment records found for this student.");
        pauseAndClear();
        return;
    }

    cout << CYAN << BOLD << "  Payment History:\n"
         << RESET;
    printSeparator();

    vector<string> headers = {"Payment ID", "Month", "Amount", "Status", "Date"};
    vector<int> widths = {13, 12, 12, 12, 15};
    printTableHeader(headers, widths);

    double totalPaid = 0;
    double totalPending = 0;

    for (const auto *p : studentPayments)
    {
        vector<string> row = {
            p->paymentId,
            p->monthYear,
            to_string((int)p->amount) + " Tk",
            p->status,
            p->date == "-" ? "N/A" : p->date};

        string color = (p->status == "paid") ? "green" : "red";
        printTableRow(row, widths, color);

        if (p->status == "paid")
            totalPaid += p->amount;
        else
            totalPending += p->amount;
    }

    printSeparator();

    cout << "\n"
         << CYAN << "  Financial Summary:\n"
         << RESET;
    cout << "  ├─ " << GREEN << "Total Paid    : " << fixed << setprecision(2)
         << totalPaid << " Tk" << RESET << "\n";
    cout << "  ├─ " << RED << "Total Pending : " << totalPending << " Tk" << RESET << "\n";
    cout << "  └─ " << YELLOW << "Grand Total   : " << (totalPaid + totalPending) << " Tk" << RESET << "\n";

    if (totalPending > 0)
        printWarning("Student has pending payments!");
    else
        printSuccess("All payments cleared!");

    pauseAndClear();
}

// ==================== ENHANCED ADMINISTRATION CALCULATE BILL ====================

void HallSystem::administrationCalculateAndGenerateBill()
{
    printHeader("CALCULATE & GENERATE BILL");

    cout << "\n";
    printMenuOption(1, "Generate for Single Student");
    printMenuOption(2, "Generate for ALL Students");
    printMenuOption(3, "Back");
    printSeparator();

    cout << YELLOW << "\n  Choice: " << RESET;

    int choice;
    if (!(cin >> choice))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        pauseAndClear();
        return;
    }

    if (choice == 3)
        return;

    string monthYear = getCurrentMonthYear();

    if (choice == 1)
    {
        cout << YELLOW << "\n  Enter Student ID: " << RESET;
        string sid;
        cin >> ws;
        getline(cin, sid);

        Student *s = findStudentById(sid);
        if (!s)
        {
            printError("Student not found!");
            pauseAndClear();
            return;
        }

        MealPlan *mp = findMealPlanByStudentId(sid);
        if (!mp || (!mp->hasLunch && !mp->hasDinner))
        {
            printWarning("Student is not subscribed to any meal.");
            pauseAndClear();
            return;
        }

        showLoadingAnimation("Calculating bill", 1);

        int daysInMonth = getDaysInMonth(monthYear);
        int offDaysCount = 0;
        for (const auto &date : mp->offDates)
        {
            if (date.substr(0, 7) == monthYear)
                offDaysCount++;
        }
        int billableDays = daysInMonth - offDaysCount;
        double totalBill = billableDays * DAILY_MEAL_RATE;

        bool exists = false;
        for (auto &p : payments)
        {
            if (p.studentId == sid && p.monthYear == monthYear)
            {
                exists = true;
                printWarning("Payment slip already exists for this month!");
                printInfo("Amount: " + to_string((int)p.amount) + " Tk | Status: " + p.status);
                break;
            }
        }

        if (!exists)
        {
            string paymentId = "PAY" + to_string(payments.size() + 1);
            payments.emplace_back(paymentId, sid, monthYear, totalBill, "pending", "-");

            printSuccess("Payment slip generated successfully!");
            cout << "\n"
                 << CYAN << "  Bill Details:\n"
                 << RESET;
            cout << "  ├─ Payment ID     : " << YELLOW << paymentId << RESET << "\n";
            cout << "  ├─ Student        : " << WHITE << s->getName() << " (" << sid << ")" << RESET << "\n";
            cout << "  ├─ Month          : " << WHITE << monthYear << RESET << "\n";
            cout << "  ├─ Total Days     : " << WHITE << daysInMonth << RESET << "\n";
            cout << "  ├─ Off Days       : " << WHITE << offDaysCount << RESET << "\n";
            cout << "  ├─ Billable Days  : " << WHITE << billableDays << RESET << "\n";
            cout << "  └─ Amount         : " << GREEN << BOLD << fixed << setprecision(2)
                 << totalBill << " Tk" << RESET << "\n";

            printBox("Bill sent to student! 📧");
        }
    }
    else if (choice == 2)
    {
        cout << "\n";
        printInfo("Generating bills for all students...");
        showLoadingAnimation("Processing", 2);
        cout << "\n\n";

        int generated = 0;
        int skipped = 0;
        int alreadyExists = 0;

        for (const auto &student : students)
        {
            string sid = student.getId();
            MealPlan *mp = findMealPlanByStudentId(sid);

            if (!mp || (!mp->hasLunch && !mp->hasDinner))
            {
                skipped++;
                continue;
            }

            bool exists = false;
            for (const auto &p : payments)
            {
                if (p.studentId == sid && p.monthYear == monthYear)
                {
                    exists = true;
                    alreadyExists++;
                    break;
                }
            }

            if (!exists)
            {
                int daysInMonth = getDaysInMonth(monthYear);
                int offDaysCount = 0;
                for (const auto &date : mp->offDates)
                {
                    if (date.substr(0, 7) == monthYear)
                        offDaysCount++;
                }
                int billableDays = daysInMonth - offDaysCount;
                double totalBill = billableDays * DAILY_MEAL_RATE;

                string paymentId = "PAY" + to_string(payments.size() + 1);
                payments.emplace_back(paymentId, sid, monthYear, totalBill, "pending", "-");

                cout << GREEN << "  ✓ " << left << setw(25) << student.getName()
                     << "(" << sid << ") - " << fixed << setprecision(2)
                     << totalBill << " Tk" << RESET << "\n";
                generated++;
            }
        }

        printSeparator();
        cout << "\n"
             << CYAN << BOLD << "  📊 Generation Summary:\n"
             << RESET;
        cout << "  ├─ " << GREEN << "Generated    : " << generated << " bills" << RESET << "\n";
        cout << "  ├─ " << YELLOW << "Already Exist: " << alreadyExists << " bills" << RESET << "\n";
        cout << "  └─ " << RED << "Skipped      : " << skipped << " (no meal subscription)" << RESET << "\n";

        if (generated > 0)
            printBox("Bills generated successfully! 💰");
    }

    pauseAndClear();
}

// ==================== BONUS: DASHBOARD ANALYTICS ====================

void HallSystem::showDashboardAnalytics()
{
    printHeader("DASHBOARD ANALYTICS");

    cout << "\n"
         << CYAN << BOLD << "  📊 System Statistics:\n"
         << RESET;

    // Students Stats
    cout << "\n  " << YELLOW << "👥 Students:\n"
         << RESET;
    cout << "  ├─ Total Students    : " << WHITE << students.size() << RESET << "\n";

    int assignedStudents = 0;
    for (const auto &s : students)
    {
        if (!s.getRoom().empty())
            assignedStudents++;
    }
    cout << "  ├─ Room Assigned     : " << GREEN << assignedStudents << RESET << "\n";
    cout << "  └─ Unassigned        : " << RED << (students.size() - assignedStudents) << RESET << "\n";

    // Rooms Stats
    cout << "\n  " << YELLOW << "🏠 Rooms:\n"
         << RESET;
    cout << "  ├─ Total Rooms       : " << WHITE << rooms.size() << RESET << "\n";

    int fullRooms = 0;
    int totalCapacity = 0;
    int totalOccupied = 0;

    for (const auto &r : rooms)
    {
        totalCapacity += r.capacity;
        totalOccupied += r.occupants.size();
        if (!r.hasSpace())
            fullRooms++;
    }

    cout << "  ├─ Full Rooms        : " << RED << fullRooms << RESET << "\n";
    cout << "  ├─ Available Rooms   : " << GREEN << (rooms.size() - fullRooms) << RESET << "\n";
    cout << "  └─ Occupancy Rate    : " << YELLOW << fixed << setprecision(1)
         << (totalCapacity > 0 ? (totalOccupied * 100.0 / totalCapacity) : 0) << "%" << RESET << "\n";

    // Complaints Stats
    cout << "\n  " << YELLOW << "📢 Complaints:\n"
         << RESET;
    int pendingComplaints = 0;
    for (const auto &c : complaints)
    {
        if (c.status == "pending")
            pendingComplaints++;
    }
    cout << "  ├─ Total Complaints  : " << WHITE << complaints.size() << RESET << "\n";
    cout << "  ├─ Pending           : " << RED << pendingComplaints << RESET << "\n";
    cout << "  └─ Resolved          : " << GREEN << (complaints.size() - pendingComplaints) << RESET << "\n";

    // Payment Stats
    cout << "\n  " << YELLOW << "💰 Payments:\n"
         << RESET;
    double totalRevenue = 0;
    double pendingRevenue = 0;
    int paidCount = 0;

    for (const auto &p : payments)
    {
        if (p.status == "paid")
        {
            totalRevenue += p.amount;
            paidCount++;
        }
        else
        {
            pendingRevenue += p.amount;
        }
    }

    cout << "  ├─ Total Payments    : " << WHITE << payments.size() << RESET << "\n";
    cout << "  ├─ Paid              : " << GREEN << paidCount << " (" << totalRevenue << " Tk)" << RESET << "\n";
    cout << "  ├─ Pending           : " << RED << (payments.size() - paidCount)
         << " (" << pendingRevenue << " Tk)" << RESET << "\n";
    cout << "  └─ Collection Rate   : " << YELLOW << fixed << setprecision(1)
         << ((totalRevenue + pendingRevenue) > 0 ? (totalRevenue / (totalRevenue + pendingRevenue) * 100) : 0)
         << "%" << RESET << "\n";

    // Meal Stats
    cout << "\n  " << YELLOW << "🍽️  Meal Plans:\n"
         << RESET;
    int lunchOnly = 0, dinnerOnly = 0, both = 0, none = 0;

    for (const auto &mp : mealPlans)
    {
        if (mp.hasLunch && mp.hasDinner)
            both++;
        else if (mp.hasLunch)
            lunchOnly++;
        else if (mp.hasDinner)
            dinnerOnly++;
        else
            none++;
    }

    cout << "  ├─ Lunch & Dinner    : " << GREEN << both << RESET << "\n";
    cout << "  ├─ Lunch Only        : " << YELLOW << lunchOnly << RESET << "\n";
    cout << "  ├─ Dinner Only       : " << YELLOW << dinnerOnly << RESET << "\n";
    cout << "  └─ No Subscription   : " << RED << none << RESET << "\n";

    printSeparator();
    pauseAndClear();
}

// ==================== ENHANCED REGISTRATION ====================

void Registration::registerAdministration()
{
    string name, id, pass, email, secQues, secAns;

    printHeader("ADMINISTRATION REGISTRATION");

    cout << YELLOW << "\n  Enter Name: " << RESET;
    getline(cin, name);

    cout << YELLOW << "  Enter Administration ID: " << RESET;
    getline(cin, id);

    cout << YELLOW << "  Enter Password: " << RESET;
    pass = readPassword();

    while (true)
    {
        cout << YELLOW << "  Enter Email (must contain @gmail.com): " << RESET;
        getline(cin, email);

        if (email.find("@gmail.com") != string::npos)
        {
            printSuccess("Email validated!");
            break;
        }
        else
        {
            printError("Invalid email! Email must contain @gmail.com");
        }
    }

    cout << YELLOW << "  Enter Security Question: " << RESET;
    getline(cin, secQues);

    cout << YELLOW << "  Enter Security Answer: " << RESET;
    getline(cin, secAns);

    showLoadingAnimation("Creating administration account", 2);

    ofstream fout(filename, ios::app);
    fout << "administration|" << name << "|" << id << "|" << pass << "|" << email << "|" << secQues << "|" << secAns << "\n";
    fout.close();

    printSuccess("Administration registered successfully!");
    printBox("Welcome to the system, " + name + "! 🎉");
    pauseAndClear();
}

bool LoginManager::administrationLogin()
{
    printHeader("ADMINISTRATION LOGIN");

    string id, pass;
    cout << YELLOW << "\n  Enter Administration ID: " << RESET;
    cin >> ws;
    getline(cin, id);

    cout << YELLOW << "  Enter Password         : " << RESET;
    pass = readPassword();

    showLoadingAnimation("Authenticating", 1);

    for (Login &l : logins)
    {
        if (l.getRole() == "administration" && l.getId() == id && l.getPassword() == pass)
        {
            clearScreen();
            printSuccess("Login successful!");
            cout << GREEN << "\n  ✨ Welcome back, " << l.getName() << "! ✨\n"
                 << RESET;
            pauseAndClear();
            return true;
        }
    }
    return false;
}

string LoginManager::studentLogin()
{
    printHeader("STUDENT LOGIN");

    string id, pass;
    cout << YELLOW << "\n  Enter Student ID: " << RESET;
    cin >> ws;
    getline(cin, id);

    cout << YELLOW << "  Enter Password  : " << RESET;
    pass = readPassword();

    showLoadingAnimation("Authenticating", 1);

    for (Login &l : logins)
    {
        if (l.getRole() == "student" && l.getId() == id && l.getPassword() == pass)
        {
            clearScreen();
            printSuccess("Login successful!");
            cout << GREEN << "\n  ✨ Welcome back, " << l.getName() << "! ✨\n"
                 << RESET;
            pauseAndClear();
            return l.getId();
        }
    }
    return "";
}

// ==================== ENHANCED HALL SYSTEM FUNCTIONS ====================

void HallSystem::addStudent()
{
    string id, name, dept, batch, email;

    printHeader("ADD NEW STUDENT");

    cout << YELLOW << "\n  Enter Student ID: " << RESET;
    cin >> ws;
    getline(cin, id);

    if (findStudentById(id))
    {
        printError("Student already exists!");
        pauseAndClear();
        return;
    }

    cout << YELLOW << "  Enter Name: " << RESET;
    getline(cin, name);

    cout << YELLOW << "  Enter Department: " << RESET;
    getline(cin, dept);

    cout << YELLOW << "  Enter Batch: " << RESET;
    getline(cin, batch);

    while (true)
    {
        cout << YELLOW << "  Enter Email (must contain @gmail.com): " << RESET;
        getline(cin, email);

        if (email.find("@gmail.com") != string::npos)
            break;
        else
            printError("Invalid email! Email must contain @gmail.com");
    }

    showLoadingAnimation("Creating student profile", 2);

    Student s(id, name, dept, batch, "");
    students.push_back(s);

    ofstream fout(studentsFile, ios::app);
    fout << s.serialize() << "\n";
    fout.close();

    ofstream lf(LOGIN_FILE, ios::app);
    lf << "student|" << name << "|" << id << "|" << id << "|" << email << "||" << "\n";
    lf.close();

    MealPlan mp(id, true, true);
    mealPlans.push_back(mp);
    ofstream mpf(mealPlansFile, ios::app);
    mpf << mp.serialize() << "\n";
    mpf.close();

    lm.loadLogins();

    printSuccess("Student added successfully!");
    cout << "\n";
    printInfo("Login ID: " + id);
    printInfo("Default Password: " + id);
    printInfo("Default Meal Plan: Lunch & Dinner ✓");
    printBox("Student profile created! 🎓");

    pauseAndClear();
}
void HallSystem::deleteStudent()
{
    printHeader("DELETE STUDENT");

    if (students.empty())
    {
        printWarning("No students to delete.");
        pauseAndClear();
        return;
    }

    // MODERN DESIGN - Perfect alignment
    cout << CYAN << BOLD << "\n  📚 STUDENT DIRECTORY\n"
         << RESET;

    // Header
    cout << MAGENTA << "  ╔═════╦════════════╦══════════════════╦════════╦═══════╦══════════╗\n"
         << RESET;
    cout << MAGENTA << "  ║ " << BOLD << " # " << " ║ " << "Student ID" << " ║ " << "Name           " << "  ║ " << "Dept  " << " ║ " << "Batch" << " ║ " << " Room   " << " ║\n"
         << RESET;
    cout << MAGENTA << "  ╠═════╬════════════╬══════════════════╬════════╬═══════╬══════════╣\n"
         << RESET;

    // Student rows
    for (size_t i = 0; i < students.size(); i++)
    {
        const auto &s = students[i];

        // Prepare display values
        string nameDisplay = s.getName();
        if (nameDisplay.length() > 15)
        {
            nameDisplay = nameDisplay.substr(0, 14) + ".";
        }

        string roomStatus = s.getRoom().empty() ? "None" : s.getRoom();
        string roomColor = s.getRoom().empty() ? RED : GREEN;

        cout << MAGENTA << "  ║ " << RESET
             << YELLOW << setw(2) << right << (i + 1) << " " << MAGENTA << " ║ " << RESET
             << CYAN << setw(10) << left << s.getId() << MAGENTA << " ║ " << RESET
             << WHITE << setw(16) << left << nameDisplay << MAGENTA << " ║ " << RESET
             << BLUE << setw(6) << left << s.getDept() << MAGENTA << " ║ " << RESET
             << GREEN << setw(5) << left << s.getBatch() << MAGENTA << " ║ " << RESET
             << roomColor << setw(8) << left << roomStatus << MAGENTA << " ║\n"
             << RESET;
    }

    cout << MAGENTA << "  ╚═════╩════════════╩══════════════════╩════════╩═══════╩══════════╝\n"
         << RESET;

    // Statistics with emojis
    int withRooms = count_if(students.begin(), students.end(), [](const Student &s)
                             { return !s.getRoom().empty(); });

    cout << "\n  " << GREEN << "✅ " << withRooms << " with rooms" << RESET
         << "  " << RED << "❌ " << (students.size() - withRooms) << " without rooms" << RESET
         << "  " << CYAN << "📊 " << students.size() << " total" << RESET << "\n";

    printSeparator();

    // Input section
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << YELLOW << "\n  🎯 Enter Student ID to delete (or 'BACK' to cancel): " << RESET;
    string id;
    getline(cin, id);

    // Handle cancellation
    if (id == "BACK" || id == "back")
    {
        printInfo("Operation cancelled.");
        pauseAndClear();
        return;
    }

    // Find student
    auto it = students.begin();
    bool found = false;
    for (; it != students.end(); ++it)
    {
        if (it->getId() == id)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        printError("Student not found!");
        cout << YELLOW << "  Please check the Student ID from the list above.\n"
             << RESET;
        pauseAndClear();
        return;
    }

    // Show confirmation with student details
    clearScreen();
    printHeader("CONFIRM DELETION");

    cout << "\n"
         << RED << BOLD << "  ⚠️  STUDENT TO BE DELETED:\n"
         << RESET;
    cout << "  ┌────────────────────────────────────────────┐\n";
    cout << "  │ " << YELLOW << "Student ID   : " << RESET << it->getId() << "\n";
    cout << "  │ " << YELLOW << "Name         : " << RESET << it->getName() << "\n";
    cout << "  │ " << YELLOW << "Department   : " << RESET << it->getDept() << "\n";
    cout << "  │ " << YELLOW << "Batch        : " << RESET << it->getBatch() << "\n";
    cout << "  │ " << YELLOW << "Room         : " << RESET << (it->getRoom().empty() ? "Not assigned" : it->getRoom()) << "\n";
    cout << "  └────────────────────────────────────────────┘\n\n";

    printWarning("❌ This action cannot be undone! All student data will be permanently deleted.");

    cout << YELLOW << "\n  Type 'DELETE' to confirm deletion, or anything else to cancel: " << RESET;
    string confirm;
    getline(cin, confirm);

    if (confirm == "DELETE")
    {
        showLoadingAnimation("Deleting student record", 2);

        string roomNo = it->getRoom();
        if (!roomNo.empty())
        {
            Room *r = findRoomByNo(roomNo);
            if (r)
                r->occupants.erase(remove(r->occupants.begin(), r->occupants.end(), id), r->occupants.end());
        }

        students.erase(it);

        // Delete from login file
        vector<string> lines;
        ifstream fin(LOGIN_FILE);
        string line;
        while (getline(fin, line))
        {
            if (line.find("student|") == 0)
            {
                stringstream ss(line);
                string parts[7];
                for (int i = 0; i < 7; i++)
                    getline(ss, parts[i], '|');

                if (parts[2] != id)
                    lines.push_back(line);
            }
            else
            {
                lines.push_back(line);
            }
        }
        fin.close();

        ofstream fout(LOGIN_FILE);
        for (const auto &l : lines)
            fout << l << "\n";
        fout.close();

        lm.loadLogins();

        clearScreen();
        printSuccess("Student deleted successfully!");
        printBox("Student record permanently removed from system 🗑️");
    }
    else
    {
        printInfo("Deletion cancelled. Student record is safe.");
    }
    pauseAndClear();
}
void HallSystem::addRoom()
{
    string rno;
    int cap;

    printHeader("ADD NEW ROOM");

    cout << YELLOW << "\n  Enter Room Number: " << RESET;
    cin >> ws;
    getline(cin, rno);

    if (findRoomByNo(rno))
    {
        printError("Room already exists!");
        pauseAndClear();
        return;
    }

    cout << YELLOW << "  Enter Capacity: " << RESET;
    cin >> cap;

    showLoadingAnimation("Creating room", 1);

    rooms.emplace_back(rno, cap);

    printSuccess("Room added successfully!");
    printInfo("Room Number: " + rno);
    printInfo("Capacity: " + to_string(cap) + " students");
    printBox("Room is ready for assignment! 🏠");

    pauseAndClear();
}

void HallSystem::assignRoom()
{
    string sid, rno;

    printHeader("ASSIGN ROOM");

    cout << YELLOW << "\n  Enter Student ID: " << RESET;
    cin >> ws;
    getline(cin, sid);

    Student *s = findStudentById(sid);
    if (!s)
    {
        printError("Student not found!");
        pauseAndClear();
        return;
    }

    printInfo("Student: " + s->getName());

    cout << YELLOW << "  Enter Room Number to assign: " << RESET;
    getline(cin, rno);

    Room *r = findRoomByNo(rno);
    if (!(*r))
    { // Using overloaded bool operator - checks if room has space
        printError("Room is full! Cannot assign.");
        pauseAndClear();
        return;
    }

    if (!(*r))
    {
        printError("Room is full! Cannot assign.");
        pauseAndClear();
        return;
    }

    showLoadingAnimation("Assigning room", 1);

    string prev = s->getRoom();
    if (!prev.empty())
    {
        Room *pr = findRoomByNo(prev);
        if (pr)
            pr->occupants.erase(remove(pr->occupants.begin(), pr->occupants.end(), sid), pr->occupants.end());
    }

    r->occupants.push_back(sid);
    s->setRoom(rno);

    printSuccess("Room assigned successfully!");
    cout << "\n";
    printInfo("Student: " + s->getName());
    printInfo("Room: " + rno);
    printInfo("Occupancy: " + to_string(r->occupants.size()) + "/" + to_string(r->capacity));
    printBox("Assignment completed! 🏠");

    pauseAndClear();
}

void HallSystem::submitComplaint(const string &studentId)
{
    string text;

    printHeader("SUBMIT COMPLAINT");

    cout << YELLOW << "\n  Enter complaint details:\n  " << RESET;
    cin >> ws;
    getline(cin, text);

    showLoadingAnimation("Submitting complaint", 1);

    string cid = "C" + to_string(complaints.size() + 1);
    complaints.emplace_back(cid, studentId, text);

    printSuccess("Complaint submitted successfully!");
    printInfo("Complaint ID: " + cid);
    printBox("We will address your concern soon! 📢");

    pauseAndClear();
}

void HallSystem::viewComplaintStatus(const string &sid)
{
    clearScreen();

    cout << MAGENTA << BOLD;
    cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
    cout << "║                 COMPLAINT STATUS & REPLIES                     ║\n";
    cout << "╚════════════════════════════════════════════════════════════════╝\n"
         << RESET;

    vector<Complaint *> myComplaints;

    // Find all complaints by this student
    for (auto &complaint : complaints)
    {
        if (complaint.hasStudentId(sid)) // Using the fixed function name
        {
            myComplaints.push_back(&complaint);
        }
    }

    if (myComplaints.empty())
    {
        cout << "\n\n";
        cout << CYAN << BOLD;
        cout << "  ╔═══════════════════════════════════════════════════════╗\n";
        cout << "  ║                   📭 NO COMPLAINTS FOUND             ║\n";
        cout << "  ╚═══════════════════════════════════════════════════════╝\n"
             << RESET;

        cout << "\n"
             << YELLOW << "  ℹ️  You haven't submitted any complaints yet.\n"
             << RESET;
        cout << "\n"
             << CYAN << "  Use 'Submit New Complaint' to report any issues.\n"
             << RESET;
    }
    else
    {
        // Sort complaints by ID (most recent first)
        sort(myComplaints.begin(), myComplaints.end(),
             [](const Complaint *a, const Complaint *b)
             {
                 return a->complaintId > b->complaintId;
             });

        cout << "\n"
             << CYAN << BOLD << "  📋 Your Complaints:\n"
             << RESET;

        for (const auto *complaint : myComplaints)
        {
            cout << "\n"
                 << MAGENTA << "  ┌─ " << "Complaint ID: " << complaint->complaintId
                 << string(40 - complaint->complaintId.length(), '-') << "┐\n"
                 << RESET;

            cout << "  │ " << YELLOW << "Status: " << RESET;
            if (complaint->status == "pending")
                cout << YELLOW << "⏳ Pending (Admin will reply soon)" << RESET << "\n";
            else
                cout << GREEN << "✅ " << complaint->status << RESET << "\n";

            cout << "  │ " << YELLOW << "Your Message:\n"
                 << RESET;
            cout << "  │   " << WHITE << complaint->text << RESET << "\n";

            // If status is not pending, show the admin reply
            if (complaint->status != "pending")
            {
                cout << "  │ " << YELLOW << "Admin Reply:\n"
                     << RESET;
                cout << "  │   " << GREEN << complaint->status << RESET << "\n";
            }

            cout << MAGENTA << "  └" << string(58, '-') << "┘\n"
                 << RESET;
        }

        cout << "\n"
             << YELLOW << "  📊 Summary: " << RESET
             << myComplaints.size() << " complaint(s) total\n";
    }

    printSeparator();
    cout << YELLOW << "\n  Press Enter to continue... " << RESET;
    cin.get();
    clearScreen();
}

void HallSystem::replyComplaint()
{
    string cid;

    printHeader("REPLY TO COMPLAINT");

    cout << YELLOW << "\n  Enter Complaint ID: " << RESET;
    cin >> ws;
    getline(cin, cid);

    for (auto &c : complaints)
    {
        if (c == cid)
        {
            cout << "\n"
                 << CYAN << "  ═══ Complaint Details ═══\n"
                 << RESET;
            cout << "  Complaint ID: " << YELLOW << c.complaintId << RESET << "\n";
            cout << "  Student ID: " << YELLOW << c.studentId << RESET << "\n";
            cout << "  Message: " << WHITE << c.text << RESET << "\n";
            cout << "  Current Status: " << (c.status == "pending" ? RED : GREEN) << c.status << RESET << "\n\n";

            cout << YELLOW << "  Enter your reply: " << RESET;
            string reply;
            getline(cin, reply);

            showLoadingAnimation("Sending reply", 1);

            c.adminReply = reply;
            c.status = "Replied"; // Or you can keep it as the reply text

            printSuccess("Reply sent successfully!");
            printBox("Student will see your reply! 📧");
            pauseAndClear();
            return;
        }
    }

    printError("Complaint not found!");
    pauseAndClear();
}

// == == == == == == == == == == ENHANCED MENUS == == == == == == == == == ==

void HallSystem::administrationMenu()
{
    // Get administration name from first login
    string administrationName = "Administration";
    for (const auto &l : lm.logins)
    {
        if (l.getRole() == "administration")
        {
            administrationName = l.getName();
            break;
        }
    }

    while (true)
    {
        displayAdministrationMenu(administrationName);

        int ch;
        if (!(cin >> ch))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (ch)
        {
        case 1:
            clearScreen();
            addStudent();
            break;
        case 2:
            clearScreen();
            displayStudentsTableEnhanced(students);
            pauseAndClear();
            break;
        case 3:
            clearScreen();
            deleteStudent();
            break;
        case 4:
            clearScreen();
            addRoom();
            break;
        case 5:
            clearScreen();
            viewRooms();
            pauseAndClear();
            break;
        case 6:
            clearScreen();
            assignRoomMenu();
            break;
        case 7:
            clearScreen();
            viewComplaints();
            pauseAndClear();
            break;
        case 8:
            clearScreen();
            replyComplaint();
            break;
        case 9:
            clearScreen();
            administrationUpdateMenu();
            break;
        case 10:
            clearScreen();
            administrationCalculateAndGenerateBill();
            break;
        case 11:
            clearScreen();
            administrationViewPayments();
            break;
        case 12:
            clearScreen();
            administrationSearchPayment();
            break;
        case 13:
            clearScreen();
            deleteAdministration();
            break;
        case 14:
            clearScreen();
            printInfo("Logging out...");
            pauseAndClear();
            return;
        case 15:
            clearScreen();
            saveAll();
            printSuccess("All data saved successfully!");
            printGoodbye();
            exit(0);
        case 16:
            clearScreen();
            showDashboardAnalytics();
            break;
        default:
            printError("Invalid choice!");
            pauseAndClear();
            break;
        }
    }
}

void HallSystem::studentMenu(const string &sid)
{
    Student *s = findStudentById(sid);
    string studentName = s ? s->getName() : "Student";

    while (true)
    {
        displayStudentMenu(studentName);

        int ch;
        if (!(cin >> ch))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        clearScreen();

        s = findStudentById(sid);

        switch (ch)
        {
        case 1:
            if (s)
            {
                displayStudentProfileEnhanced(*s);
            }
            pauseAndClear();
            break;
        case 2:
            clearScreen();
            editStudentProfile(sid);
            break;
        case 3:
            if (!s)
            {
                printError("Student not found!");
                pauseAndClear();
                break;
            }
            {
                printHeader("ROOM INFORMATION");
                string rn = s->getRoom();
                if (rn.empty())
                    printWarning("No room assigned yet.");
                else
                {
                    Room *r = findRoomByNo(rn);
                    if (r)
                    {
                        cout << "\n";
                        cout << GREEN << "  🚪 Room Number: " << RESET << r->roomNo << "\n";
                        cout << YELLOW << "  👥 Capacity: " << RESET << r->capacity << "\n";
                        cout << BLUE << "  👤 Occupied: " << RESET << r->occupants.size() << "\n";

                        if (!r->occupants.empty())
                        {
                            cout << "\n"
                                 << CYAN << "  Roommates:\n"
                                 << RESET;
                            for (const auto &occupantId : r->occupants)
                            {
                                if (occupantId != sid)
                                {
                                    Student *roommate = findStudentById(occupantId);
                                    if (roommate)
                                        cout << "  • " << roommate->getName() << " (" << occupantId << ")\n";
                                }
                            }
                        }
                    }
                    else
                        printError("Room info missing!");
                }
                pauseAndClear();
            }
            break;

        case 4:
        {
            bool backToMain = false;
            while (!backToMain)
            {
                clearScreen();
                cout << MAGENTA << BOLD;
                cout << "\n╔════════════════════════════════════════════════════════════════╗\n";
                cout << "║                       COMPLAINT                                ║\n";
                cout << "╚════════════════════════════════════════════════════════════════╝\n"
                     << RESET;

                cout << "\n"
                     << CYAN << BOLD << "  📋 Options:\n"
                     << RESET;
                printMenuOption(1, "📝 Submit New Complaint", false);
                printMenuOption(2, "👀 View Complaint Status & Replies", false);
                printMenuOption(3, "🔙 Back to Student Menu", true);

                printSeparator();
                cout << YELLOW << "\n   Enter your choice (1-3): " << RESET;

                int complaintChoice;
                if (!(cin >> complaintChoice))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (complaintChoice)
                {
                case 1:
                    submitComplaint(sid);
                    break;
                case 2:
                    viewComplaintStatus(sid);
                    break;
                case 3:
                    backToMain = true;
                    break;
                default:
                    printError("Invalid choice!");
                    pauseAndClear();
                    break;
                }
            }
            break;
        }
        case 5:
            clearScreen();
            requestRoomSwap(sid);
            break;
        case 6:
            clearScreen();
            viewSwapRequests(sid);
            break;
        case 7:
            clearScreen();
            manageMealPlan(sid);
            break;
        case 8:
            clearScreen();
            viewTodayMenu(sid);
            break;
        case 9:
            clearScreen();
            markMealOff(sid);
            break;
        case 10:
            clearScreen();
            viewMealPlan(sid);
            break;
        case 11:
            clearScreen();
            studentViewPaymentSlips(sid);
            break;
        case 12:
            clearScreen();
            makePayment(sid);
            break;
        case 13:
            clearScreen();
            changeStudentPassword(sid);
            break;
        case 14:
            clearScreen();
            printInfo("Logging out...");
            printBox("See you soon! 👋");
            pauseAndClear();
            return;
        default:
            printError("Invalid choice!");
            pauseAndClear();
        }
    }
}

// ------------------ Main Function ------------------
int main()
{
    try
    {
        HallSystem hs;
        hs.start();
        return 0;
    }
    catch (...)
    {
        cout << "❌ Unknown error occurred!" << endl;
        return 1;
    }
}
