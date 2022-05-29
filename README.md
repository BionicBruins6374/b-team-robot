# b-team-robot

## Contributing Rules

The following rules provide a format and structure for the code written here,
so that different coding styles do not conflict and so that code is easily readable and maintainable.

All contributors to this repo must follow these rules.
In addition, any changes made must be made as a pull request.
Do not push code to the main branch.

Ensure that your branch name is fairly descriptive. Use `kebab-case` for branch names with more than one word.

## Code Guidelines

- `PascalCase` for types and type-level constructs (structs, classes, enums, aliases, concepts, etc)
- `snake_case` for values (variables, functions, fields, etc)
- `UPPER_SNAKE_CASE` for constants (not necessarily anything marked const)
- Use tabs for indentation, spaces for alignment
- Run `clang-format` for basic formatting (config: [.clang-format](/.clang-format))
- Use `constexpr` variables for constants,
	- For configuration, measurements, etc, use namespaces (`config`, `measures`, etc)
- Generally prefer OOP to encapsulate raw PROS utilities (Arm, Claw, LineTracker, etc)
- Prefer PROS C APIs over C++ APIs for basic functions like motors
- Use east `const`, with characters on the `const` (e.g., `std::string const& my_string`, `void my_func(int const* my_int) {}`)
- Aggressively mark variables, parameters, and methods `const`. If it can be `const`, it should be.
