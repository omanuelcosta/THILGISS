class InputNotSupported : public std::exception {
private:
    int value;

public:
    InputNotSupported(int val) : value(val) {}

    int what() {
        return value;
    }
};

class InputNoNameType : public std::exception {
private:
    std::string message;

public:
    InputNoNameType(std::string msg) : message(msg) {}

    std::string what() {
        return message;
    }
};