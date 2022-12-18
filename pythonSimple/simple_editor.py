#!/usr/bin/env python3
import curses

#todo DEL_REVERSED

EXIT = 24  # Ctrl+X
SAVE = 15  # Ctrl+O
ESC = 27
DEL = 263  # Backspace MacBook
DEL_REVERSED = 330  # Fn + Backspace Macbook
CHANGE_SIZE = 410  # when terminal size is being modified
ENTER = 10  # Enter

action_map = {
        curses.KEY_UP: "shift_up",
        curses.KEY_DOWN: "shift_down",
        curses.KEY_LEFT: "shift_left",
        curses.KEY_RIGHT: "shift_right",
        EXIT: "exit", ESC: "exit",
        curses.KEY_BACKSPACE: "delete_char",
        DEL_REVERSED: "delete_char_reversed",
        ENTER: "insert_new_line",
        SAVE: "save_file",
        CHANGE_SIZE: "change_size"
    }


class TextEditor:

    outputLines = []
    screen = None

    def __init__(self, file_name=None):
        self.screen = curses.initscr()
        curses.noecho()
        curses.cbreak()
        self.screen.keypad(1)
        self.log_file = open('log.txt', 'w')

        self.top_line_number = 0
        self.left_line_number = 0
        self.x, self.y = 0, 0
        self.line_amount = 0
        try:
            with open(file=file_name, mode="a",) as fo:
                pass
            self.file_name = file_name
        except Exception as e:
            print(e)

        self.read_lines()
        self.y_max, self.x_max = self.screen.getmaxyx()
        self.run()

    def get_yx(self):
        """
        Return absolute value of cursor
        :return: tuple(y, x) -> int, int
        """
        x = self.x + self.left_line_number
        y = self.y + self.top_line_number
        return y, x

    def insert_char(self, char_code):
        self.log('add_char', chr(char_code))

        y, x = self.get_yx()
        line = self.outputLines[y]
        line = line[:x] + chr(char_code) + line[x:]
        self.outputLines[y] = line
        self.shift_right()

    def delete_char(self):
        y, x = self.get_yx()
        current_line = self.outputLines[y]
        if x != 0:  # middle of sentence
            modified_line = current_line[:x-1] + current_line[x:]  # deleted symbol
            self.outputLines[y] = modified_line
            self.shift_left()
            return
        elif y == 0:
            return
        elif len(current_line) == 0:  # delete empty line
            if y == self.line_amount - 1:  # don't delete last line
                self.shift_left()
                return
            self.shift_left()
            del self.outputLines[y]

        else:  # 0 position, have to merge
            previous_line = self.outputLines[y-1]
            previous_line += current_line
            self.shift_left()
            self.outputLines[y-1] = previous_line
            del self.outputLines[y]

        if self.top_line_number:
            self.top_line_number -= 1
            self.y += 1
        self.line_amount -= 1

    def delete_char_reversed(self):
        pass

    def insert_new_line(self):
        y, x = self.get_yx()
        current_line = self.outputLines[y]
        new_line = ""
        if x != len(current_line)+1:  # fix. Line is empty if value unver cursor is u.
            self.outputLines[y] = current_line[:x]
            new_line = current_line[x:]
        self.outputLines.insert(y + 1, new_line)
        self.shift_right()
        self.line_amount += 1

    def save_file(self):
        with open(self.file_name, "w") as fo:
            for line in self.outputLines[:-1]:  # Do not save last empty line
                fo.write(line + "\n")

    def change_size(self):
        self.y_max, self.x_max = self.screen.getmaxyx()
        self.x, self.y = 0, 0
        self.top_line_number = 0
        self.left_line_number = 0

    def run(self):
        while True:
            self.update_screen()
            self.screen.move(self.y, self.x)

            c = self.screen.getch()
            if c in action_map.keys():
                action = getattr(self, action_map[c])
                action()
            else:
                self.insert_char(c)
                # print(c)  # to find out new combinations : )

    def shift_left(self):
        if self.x == 0:
            if self.left_line_number != 0:
                self.left_line_number -= 1
            else:
                if self.y + self.top_line_number == 0:
                    return
                self.shift_up()
                upper_line_len = len(self.outputLines[self.y+self.top_line_number])
                if upper_line_len > self.x_max-3:  # last char before $ sign
                    self.x = self.x_max - 4
                else:
                    self.x = upper_line_len
        else:
            self.x -= 1
        self.log('change_location')

    def shift_right(self):
        y, x = self.get_yx()
        if y == self.line_amount:
            return
        line_len = len(self.outputLines[y])
        if x != line_len:
            if self.x < self.x_max - 4:
                self.x += 1
            else:
                self.left_line_number += 1
        else:
            self.x = 0
            self.left_line_number = 0
            self.shift_down()
        self.log('change_location')

    def shift_up(self):
        self.left_line_number = 0
        if self.y > 0:
            self.y -= 1
        else:
            if self.top_line_number != 0:
                self.top_line_number -= 1
        if len(self.outputLines[self.y + self.top_line_number]) < self.x:
            self.x = len(self.outputLines[self.y + self.top_line_number])
        self.log('change_location')

    def shift_down(self):
        y, x = self.get_yx()
        self.left_line_number = 0
        if self.y+1 < self.y_max:  # curse is in middle
            if self.y+1 == self.line_amount:
                return
            self.y += 1
        else:
            if self.y + self.top_line_number == self.line_amount-1:
                return
            self.top_line_number += 1
        if len(self.outputLines[self.y + self.top_line_number]) < self.x:
            self.x = len(self.outputLines[self.y + self.top_line_number])
        self.log('change_location')

    def read_lines(self):
        self.outputLines = [x.strip("\n") for x in open(self.file_name).readlines()]
        self.outputLines.append("")  # Append new line to prevent errors in scrolling
        self.line_amount = len(self.outputLines)

    def update_screen(self):
        # clear screen
        self.screen.erase()

        # now paint the rows
        top = self.top_line_number
        bottom = top + self.y_max
        left = self.left_line_number
        right = left + self.x_max - 4
        for (index, line,) in enumerate(self.outputLines[top:bottom]):
            line_cut = line[left:right+1]
            if line_cut != line[left:]:  # end is cut
                line_cut += " $"
            self.screen.addstr(index, 0, line_cut)
        self.screen.refresh()

    log_map = {
        'change_location': '0',
        'add_char': '1',
        'delete_char': '2',
        'add_line': '3',
        'delete_line': '4',
    }

    def log(self, key, char=None):
        pos = str(self.get_yx())
        log = (TextEditor.log_map[key] + " " + pos)
        if key == 'add_char':
            log += " " + char
        log += '\n'
        self.log_file.write(log)

    @staticmethod
    def restore_screen():
        curses.initscr()
        curses.nocbreak()
        curses.echo()
        curses.endwin()

    # catch any weird termination situations
    def __del__(self):
        self.restore_screen()


if __name__ == '__main__':
    import sys
    if len(sys.argv)==1:
        print("ERROR: Need to supply a filename as first argument!", file=sys.stderr)
        sys.exit(1)
    else:
        print("Use ESC / Ctrl+X / Ctrl+C to exit and Ctrl+O to save file!")
        ih = TextEditor(file_name=sys.argv[1])
