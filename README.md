# b-team-robot

## Contributing Rules:
The following rules provide a format and structure for the code written here, so that different coding styles do not conflict and so that code is easily readable and maintainable.
ALL contributors to this repo must follow these rules. In addition, any changes made must be made as a pull request. DO NOT push code to the main branch, even for admins. 
Any PR that does not following the following rules will be denied merge.
- PascalCase for Types, Classes, Structs
- snake_case for any variables, methods, etc.
- Tabs (size: 4)
- Clang Formatting ([.clang-format](/.clang-format)) 
- Constexpr variables for constant values, INSIDE a namespace (`constexpr int config::my_const = 0;`)
