**Semester project for subject `Network Algorithmics`** (**Algoritmy v sieťach - `AvS`**)

## Project
- It's Authoritative DNS
- written in C
- stable (?)
- with wide variety of $features = {sanity \over 13 weeks}$

## Features
- TOML configuration file
- Zone files (using RFC-1035 format) with custom parser
- Support for basic DNS resource record types (A/NS/CNAME/SOA/MX/TXT/AAAA)
- Hierarchically designed database:
    - `Trie` - to store zones (keyed by zone name, each zone represented in form of `HashTable`)
    - `HashTable` that is keyed by each unique domain name in said zone (value stores `DomainNameDB`)
    - `DomainNameDB` that is keyed by a RR type (A/TXT/..). Value contains final `ResourceRecord` chain (chain in case where multiple records of the same domainname+type exist)

## Development
### Naming conventions
We're trying to mostly follow the sane GTK+ **naming** conventions in here, so basically:
- All macros and constants in caps: `MAX_BUFFER_SIZE`, `TRACKING_ID_PREFIX`.
- Struct names, enums and typedef's in camelcase: `ArrayList`, `TrackingOrder`.
- Functions that operate on structs: classic C style: `arraylist_init()`, `print_something()` (note the prefix for struct-related functions).
- Local variables/function parameters: `snake_case` please please please please please please

## Notes
- This project is just a Proof of Concept
- For fancy project structure, see [The Pitchfork Layout](https://github.com/vector-of-bool/pitchfork)
