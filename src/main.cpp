#include <bits/stdc++.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <regex>

using namespace std;

class User {
public:
    string username;
    string password;

    User(const string& uname, const string& pass) : username(uname), password(pass) {}
};

class Question {
public:
    string questionText;
    vector<string> options;
    int correctAnswer;

    Question(const string& qText, const vector<string>& opts, int correct)
        : questionText(qText), options(opts), correctAnswer(correct) {}
};

class UserDataHandler {
private:
    static constexpr const char* USER_DATA_FILE = "users.txt";

public:
    static vector<User> loadUsers() {
        vector<User> users;
        ifstream file(USER_DATA_FILE);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string uname, pass;
            getline(ss, uname, ',');
            getline(ss, pass);
            users.emplace_back(uname, pass);
        }
        return users;
    }

    static void saveUsers(const vector<User>& users) {
        ofstream file(USER_DATA_FILE);
        for (const auto& user : users) {
            file << user.username << "," << user.password << "\n";
        }
    }
};

class Exam {
protected:
    User user;
    int score;
    int totalQuestions;

public:
    Exam(const User& usr) : user(usr), score(0), totalQuestions(0) {}

    void startExam(vector<Question>& questions) {
        totalQuestions = questions.size();
        int qNo = 1;
        for (auto& question : questions) {
            cout << "Question " << qNo++ << ": " << question.questionText << "\n";
            for (size_t i = 0; i < question.options.size(); ++i) {
                cout << i + 1 << ". " << question.options[i] << "\n";
            }
            cout << "Enter your answer (1-" << question.options.size() << "): ";
            int ans;
            cin >> ans;
            if (ans == question.correctAnswer) {
                cout << "Correct!\n";
                score++;
            } else {
                cout << "Incorrect. Correct answer is: "
                     << question.options[question.correctAnswer - 1] << "\n";
            }
            cout << "---------------------------------------------\n";
        }
    }

    void displayResult() {
        cout << "Your score: " << score << " out of " << totalQuestions << "\n";
        double percentage = (double(score) / totalQuestions) * 100;
        cout << "Your Percentage is: " << percentage << "%\n";

        if (percentage >= 90)
            cout << "Your Grade is: A\nCongratulations! You passed.\n";
        else if (percentage >= 80)
            cout << "Your Grade is: B\nCongratulations! You passed.\n";
        else if (percentage >= 70)
            cout << "Your Grade is: C\nCongratulations! You passed.\n";
        else if (percentage >= 60)
            cout << "Your Grade is: D\nCongratulations! You passed.\n";
        else
            cout << "Your Grade is: F\nSorry! You have failed.\n";
    }

    int getScore() const { return score; }
};

bool isStrongPassword(const string& password) {
    return regex_match(password, regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).{8,}$"));
}

User registerUser(vector<User>& users) {
    string uname, password;
    int attempts = 2;
    cout << "Enter your username: ";
    cin.ignore();
    getline(cin, uname);

    while (attempts > 0) {
        cout << "\nPassword must:\n"
             << "- Be at least 8 characters\n"
             << "- Include uppercase, lowercase, digit, and special character\n";
        cout << "Enter your password: ";
        getline(cin, password);

        if (isStrongPassword(password)) {
            User newUser(uname, password);
            users.push_back(newUser);
            cout << "Registration successful!\n";
            return newUser;
        } else {
            attempts--;
            cout << "Weak password. ";
            if (attempts > 0)
                cout << "You have " << attempts << " attempts left.\n";
            else {
                cout << "All attempts used. Try again tomorrow.\n";
                exit(0);
            }
        }
    }
    return User("", "");
}

User loginUser(const vector<User>& users) {
    string uname, password;
    cin.ignore();
    cout << "Enter your username: ";
    getline(cin, uname);
    cout << "Enter your password: ";
    getline(cin, password);

    for (const auto& user : users) {
        if (user.username == uname && user.password == password) {
            cout << "Login successful!\n";
            return user;
        }
    }
    cout << "Login failed. Try again.\n";
    return loginUser(users);
}

User loginOrRegister(vector<User>& users) {
    while (true) {
        int choice;
        cout << "\n1. Register\n2. Login\nEnter choice: ";
        cin >> choice;
        if (choice == 1)
            return registerUser(users);
        else if (choice == 2)
            return loginUser(users);
        else
            cout << "Invalid choice. Try again.\n";
    }
}

vector<Question> selectRandomQuestions(vector<Question>& questions, int n) {
    random_shuffle(questions.begin(), questions.end());
    vector<Question> selected(questions.begin(), questions.begin() + min(n, int(questions.size())));
    return selected;
}

void saveExamResult(const User& user, int score, int total) {
    ofstream file("exam_results.txt", ios::app);
    double percent = (double(score) / total) * 100;
    file << user.username << "," << score << "," << fixed << setprecision(2) << percent << "%\n";
    file.close();
}

int main() {
    srand(unsigned(time(0)));
    vector<User> users = UserDataHandler::loadUsers();
    User currentUser = loginOrRegister(users);

    vector<Question> allQuestions = {
        Question("What is 2 + 2?", {"3", "4", "5", "6"}, 2),
        Question("Capital of India?", {"Mumbai", "Chennai", "Gujarat", "Delhi"}, 4),
        Question("First Indian cricketer to take two 5-wicket hauls in World Cup?", {"Bumrah", "Shami", "Ashwin", "Pandya"}, 2),
        Question("First PM of India?", {"Gandhi", "Sardar Patel", "Nehru", "Bose"}, 3),
        Question("Sardar Patel's nickname?", {"Father of Nation", "Iron Man", "Mahatma", "Freedom Fighter"}, 2),
        Question("Current PM of India?", {"Modi", "Rahul", "Manmohan", "Kejriwal"}, 1),
        Question("Founder of Nirma University?", {"Karsanbhai", "Ambani", "Tata", "Premji"}, 1),
        Question("OOP course coordinator?", {"Sanjay", "Ajay", "RT", "RRN"}, 2),
        Question("How many continents?", {"3", "4", "5", "7"}, 4),
        Question("Gujarat Foundation Day?", {"1/05/1960", "1/05/1965", "1/05/1959", "1/05/1962"}, 1)
    };

    int numQuestions;
    cout << "Enter number of questions you want to answer: ";
    cin >> numQuestions;

    vector<Question> selected = selectRandomQuestions(allQuestions, numQuestions);
    Exam exam(currentUser);
    exam.startExam(selected);
    exam.displayResult();

    UserDataHandler::saveUsers(users);
    saveExamResult(currentUser, exam.getScore(), numQuestions);

    return 0;
}
