#include <iostream>
#include <string>
#include <vector>
#include <utility>

class Book {
public:
    Book(std::string title, std::string author)
        : title_(std::move(title)), author_(std::move(author)) {
    }

    const std::string& getTitle() const { return title_; }
    const std::string& getAuthor() const { return author_; }

private:
    std::string title_;
    std::string author_;
};

class IBookRepository {
public:
    virtual void save(const Book& book) = 0;
    virtual ~IBookRepository() = default;
};

class INotifier {
public:
    virtual void notifyBookAdded(const Book& book) = 0;
    virtual ~INotifier() = default;
};

class DatabaseBookRepository : public IBookRepository {
public:
    void save(const Book&) override {
        std::cout << "Saving book to database..." << std::endl;
    }
};

class EmailNotifier : public INotifier {
public:
    void notifyBookAdded(const Book&) override {
        std::cout << "Sending email notification about the new book..." << std::endl;
    }
};

class SmsNotifier : public INotifier {
public:
    void notifyBookAdded(const Book&) override {
        std::cout << "Sending SMS about the new book..." << std::endl;
    }
};

class Library {
public:
    Library(IBookRepository& repository, INotifier& notifier)
        : repository_(repository), notifier_(notifier) {
    }

    void addBook(const Book& book) {
        books_.push_back(book);
        repository_.save(book);
        notifier_.notifyBookAdded(book);
    }

    void displayBooks() const {
        for (const auto& book : books_) {
            std::cout << "Title: " << book.getTitle()
                << ", Author: " << book.getAuthor() << std::endl;
        }
    }

private:
    std::vector<Book> books_;
    IBookRepository& repository_;
    INotifier& notifier_;
};

int main() {
    DatabaseBookRepository repo;

    std::cout << "=== Email ===" << std::endl;
    EmailNotifier email;
    Library library(repo, email);
    library.addBook(Book("The Catcher in the Rye", "J.D. Salinger"));
    library.addBook(Book("To Kill a Mockingbird", "Harper Lee"));
    library.displayBooks();

    std::cout << std::endl << "=== SMS (замена без изменения Library) ===" << std::endl;
    SmsNotifier sms;
    Library library2(repo, sms);
    library2.addBook(Book("1984", "George Orwell"));
    library2.displayBooks();

    return 0;
}