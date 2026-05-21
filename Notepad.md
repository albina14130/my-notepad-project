# Notepad Application Documentation

## Implemented Features
- **Exception Handling**: Custom hierarchy (`NotepadException`, etc.) used for file I/O with mandatory "Error" dialog titles.
- **Spell Checker**: Real-time red underlining using `QSyntaxHighlighter` and a dictionary of 370k+ words.
- **Context Menu**: Right-click on misspelled words provides replacement suggestions.
- **Word Frequency**: Analysis tool available via Tools menu.

## Bonus Features
1. **Cursor Tracking**: Live Line and Column indicator in the status bar.
2. **Font Dialog**: Users can change font family and size via `Format > Font...`.
3. **Color Picker**: Support for changing text color via `Format > Text Color...`.

## Setup Instructions
- Set the Working Directory in CLion to `$ProjectFileDir$` to load `data/words.txt` and toolbar icons.