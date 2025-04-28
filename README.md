# Operating Systems Course Assignments

## Advanced Shell Project

The Advanced Shell project is a robust, custom shell implementation written in C, designed to extend traditional shell functionality with specialized commands tailored for managing store flyers, particularly for a "Black Friday" campaign. 
This project integrates standard shell command execution with a suite of custom commands, showcasing advanced process management, file I/O, and string manipulation in a modular, multi-file structure.

The Advanced Shell serves as an interactive command-line interface that:

- Executes standard shell commands (e.g., `ls`, `date`) by forking processes and delegating to the system shell using `fork()` and `execvp()`.
- Provides custom commands for creating, retrieving, and managing store flyers and customer orders, stored in a dedicated "Black_Friday" directory.
- Demonstrates a practical application of C programming in a shell-like environment, with separate C files for each command and helper functions for modularity.
- The project is compiled and executed via a shell script (`Advanced_Shell.sh`), which ties together the various components into a cohesive executable (`advancedShell`).

### Custom Commands

**`CreateFlyer`**

- Syntax: `CreateFlyer [company name] [discount]`
- Functionality:
  - Prompts the user to input item names and prices (e.g., "Laptop 999.99") until `stop` is entered.
  Validates that prices are numeric (integers or floats).
  Offers a choice of predefined messages (e.g., "Enjoyed") or a custom message.
  Saves the flyer as `Black_Friday/[company name].txt` with the format:

```
[company name] Sale
[discount]% off

[item1 name]..........[price1]NIS
[item2 name]..........[price2]NIS
[message]
```
- Adds the company name to `camp_partic.txt` if not already present.

Example:
```
AdvShell>CreateFlyer "Tech Store" 20
Insert item name and item cost or 'stop': Laptop 999.99
Insert item name and item cost or 'stop': Mouse 49.99
Insert item name and item cost or 'stop': stop
Choose a message: 1
Flyer Created
```

**`GetFlyer`**

- Syntax: `GetFlyer [company name]`
- Functionality:
  - Reads and displays the contents of `Black_Friday/[company name].txt`.
  - Returns an error if the flyer doesn’t exist.

Example:
```
AdvShell>GetFlyer "Tech Store"
Flyer content:
Tech Store Sale
20% off

Laptop..........999.99NIS
Mouse..........49.99NIS
Enjoyed
```

**`GetNumComp`**

- Syntax: `GetNumComp`
- Functionality:
  - Counts the number of lines in `camp_partic.txt`, representing participating companies.
  - Outputs the total (e.g., "5 companies take part in the Campaign").
- Implementation:
  - Uses `read()` to load the file into a buffer and counts newline characters.

**`MakeOrder`**

- Syntax: MakeOrder [company name] [customer name]
- Functionality:
  - Verifies the company exists in `camp_partic.txt`.
  - Displays the flyer using `GetFlyer()`.
  - Prompts for item names and quantities (e.g., "Laptop 2") until `stop`.
  - Validates items against the flyer and ensures quantities are numeric.
  - Calculates the total cost with the discount applied.
  - Saves the order as `Black_Friday/Order [company name]/[customer name].txt` with the format:
```
[company name] Order

[item1 name] - [quantity1]
[item2 name] - [quantity2]
Total Price: [discounted total] NIS
Date: [YYYY-MM-DD]
```
- Creates the order directory if it doesn’t exist using a forked shell command.

Example:
```
AdvShell>MakeOrder "Tech Store" Alice
Flyer content: [flyer displayed]
Insert item name and quantity or 'stop': Laptop 2
Insert item name and quantity or 'stop': stop
To confirm your order, insert 'confirm' or 'cancel': confirm
Order Created
```

**`GetOrderNum`**

- Syntax: `GetOrderNum [company name]`
- Functionality:
  - Counts the number of order files in `Black_Friday/Order [company name]/`.
  - Outputs the result (e.g., "Tech Store ---- 3 Orders").
  - Returns an error if the company or order directory doesn’t exist.

**`GetSummary`**

- Syntax: `GetSummary`
- Functionality:
  - Displays the total number of participating companies (via `GetNumComp()`).
  - Lists each company and its order count (via `GetOrderNum()`).

Example:
```
AdvShell>GetSummary
2 companies take part in the Campaign
Tech Store ---- 3 Orders
Book Shop ---- 1 Orders
```

### Usage Example

Here’s a sample interaction with the Advanced Shell:
```
AdvShell>CreateFlyer "Tech Store" 20
Insert item name and item cost or 'stop': Laptop 999.99
Insert item name and item cost or 'stop': stop
Choose a message: 1
Flyer Created

AdvShell>GetFlyer "Tech Store"
Flyer content:
Tech Store Sale
20% off

Laptop..........999.99NIS
Enjoyed

AdvShell>MakeOrder "Tech Store" Alice
[Flyer displayed]
Insert item name and quantity or 'stop': Laptop 1
Insert item name and quantity or 'stop': stop
To confirm your order, insert 'confirm' or 'cancel': confirm
Order Created

AdvShell>GetSummary
1 companies take part in the Campaign
Tech Store ---- 1 Orders

AdvShell>exit
Goodbye
```

## Multithreading

**threadMatrix.c**: Creates an array of 10,000 random numbers (0-100), prompts for a number to search, and uses two threads to search the left and right halves concurrently; the first thread to find the number cancels the other and prints the index.

## Creating Processes in Windows and Using Pipes

### Using Pipes for Processes Communication

**Pipe_Son.c**: A Windows child process that reads a string from a pipe, modifies it by swapping case and replacing commas/spaces with semicolons, and writes the result back to another pipe.

**Pipe_Father.c**: A Windows parent process that creates two pipes, prompts for a string, launches "Pipe_Son.exe" with a pipe handle, sends the string to the child, waits for the response, and prints the modified string.

### Creating Processes in Windows

**Fibbo_Son.c**: A child program that prints "I am the son!" and generates a Fibonacci sequence up to the number provided as a command-line argument, ensuring the input is positive.

**Fibbo_Father.c**: A Windows program that prompts for a number, constructs a command to run "Fibbo_Son.exe" with that number, creates a child process to execute it, waits for completion, and then closes the handles.

## Creating Processes in Linux

**doFile_shell.c**: Reads a file, splits each line into words, and executes a single shell script to process keywords ("file", "dir", "copy") with their arguments, running touch, mkdir, or cp commands.

**doFile_fork.c**: Reads a file, splits each line into words, and uses fork() to spawn child processes that execute shell commands (touch, mkdir, or cp) based on keywords ("file", "dir", "copy").

## Using Shell Scripts

**checkString.c**: Checks if a given word (first argument) can be formed using single-character arguments provided after it, printing whether the word "exists" or "not exists" based on matching all letters.

**subtext.c**: Executes a shell command to search for a word in a file, printing if the file contains the word or "No matches found!" based on the grep result.

**shell_script.sh**: Creates a directory, generates a file "etgar.txt" with the first and last 10 lines of an input file, and removes write permissions from the file.

**runCheck.sh**: Compiles and runs "checkString.c" with two test cases to check if the words "test" and "xyz" can be formed from given character sets.

**catByAt.c**: Splits a string (first argument) by '@' delimiters and prints each resulting substring on a new line.

**runCat.sh**: Compiles and runs "catByAt.c" with two test strings, splitting "Israel1990@gmail@com" and "Testing@The@Program" by '@' and printing the results.
