**Semester project for subject `Network Algorithmics`** (**Algoritmy v sieťach - `AvS`**)

## Project
- It's Authoritative DNS
- written in C
- stable (?)
- with wide variety of $features = {sanity \over 13 weeks}$

## Features
- GitHub repository

## Development
### Naming conventions
We're trying to mostly follow the sane GTK+ **naming** conventions in here, so basically:
- All macros and constants in caps: `MAX_BUFFER_SIZE`, `TRACKING_ID_PREFIX`.
- Struct names, enums and typedef's in camelcase: `ArrayList`, `TrackingOrder`.
- Functions that operate on structs: classic C style: `array_list_init()`, `print_something()` (note the prefix for struct-related functions).
- Local variables/function parameters: `snake_case` please please please please please please

## Notes
- This project is just a Proof of Concept
- For fancy project structure, see [The Pitchfork Layout](https://github.com/vector-of-bool/pitchfork)
