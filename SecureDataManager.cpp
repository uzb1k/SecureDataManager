#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h> // Для задания прав доступа к файлам

class User {
private:
    std::string _name;
    std::string _login;
    std::string _pass;

public:
    User(const std::string& name, const std::string& login, const std::string& pass)
        : _name(name), _login(login), _pass(pass) {}

    void saveToFile(std::ofstream& file) const {
        file << _name << '\n' << _login << '\n' << _pass << '\n';
    }

    static User loadFromFile(std::ifstream& file) {
        std::string name, login, pass;
        std::getline(file, name);
        std::getline(file, login);
        std::getline(file, pass);
        return User(name, login, pass);
    }
};

class Message {
private:
    std::string _text;
    std::string _sender;
    std::string _receiver;

public:
    Message(const std::string& text, const std::string& sender, const std::string& receiver)
        : _text(text), _sender(sender), _receiver(receiver) {}

    void saveToFile(std::ofstream& file) const {
        file << _text << '\n' << _sender << '\n' << _receiver << '\n';
    }

    static Message loadFromFile(std::ifstream& file) {
        std::string text, sender, receiver;
        std::getline(file, text);
        std::getline(file, sender);
        std::getline(file, receiver);
        return Message(text, sender, receiver);
    }
};

void setFilePermissions(const char* filename) {
    // Устанавливаем права доступа, чтобы файл мог читать и писать только текущий пользователь
    chmod(filename, S_IRUSR | S_IWUSR);
}

int main() {
    const char* userFileName = "users.txt";
    const char* messageFileName = "messages.txt";

    // Создаем векторы для хранения объектов
    std::vector<User> users;
    std::vector<Message> messages;

    // Считывание данных из файлов
    {
        std::ifstream userFile(userFileName);
        if (userFile.is_open()) {
            while (userFile.peek() != EOF) {
                User user = User::loadFromFile(userFile);
                users.push_back(user);
            }
            userFile.close();
        }

        std::ifstream messageFile(messageFileName);
        if (messageFile.is_open()) {
            while (messageFile.peek() != EOF) {
                Message message = Message::loadFromFile(messageFile);
                messages.push_back(message);
            }
            messageFile.close();
        }
    }

    // Пример добавления нового пользователя и сообщения
    users.emplace_back("John Doe", "jdoe", "password123");
    messages.emplace_back("Hello, how are you?", "jdoe", "alice");

    // Запись данных в файлы
    {
        std::ofstream userFile(userFileName);
        if (userFile.is_open()) {
            for (const auto& user : users) {
                user.saveToFile(userFile);
            }
            userFile.close();
        }
        setFilePermissions(userFileName); // Устанавливаем права доступа для файла пользователей

        std::ofstream messageFile(messageFileName);
        if (messageFile.is_open()) {
            for (const auto& message : messages) {
                message.saveToFile(messageFile);
            }
            messageFile.close();
        }
        setFilePermissions(messageFileName); // Устанавливаем права доступа для файла сообщений
    }

    return 0;
}