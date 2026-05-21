# ifndef NOTEPAD_EXCEPTION_H
#define NOTEPAD_EXCEPTION_H

#include <stdexcept>
#include <string>

class NotepadException : public std::runtime_error {
public:
    explicit NotepadException(const std::string& message) : std::runtime_error(message) {}
};

class FileNotFoundException : public NotepadException {
public:
    explicit FileNotFoundException(const std::string& path)
            : NotepadException("File not found: " + path) {}
};

class FileReadException : public NotepadException {
public:
    explicit FileReadException(const std::string& path)
            : NotepadException("Failed to read file: " + path) {}
};

class FileWriteException : public NotepadException {
public:
    explicit FileWriteException(const std::string& path)
            : NotepadException("Failed to write to file: " + path) {}
};

#endif