# C++ Git Clone Project

## Overview

This project is a comprehensive implementation of a version control system inspired by Git, built from scratch in C++. The goal is to understand the core mechanisms of version control by reimplementing its fundamental concepts.

## Project Architecture

### Core Components
- **GitObject**: Abstract base class for all version-controlled objects
- **Blob**: Represents file contents
- **Tree**: Represents directory structures
- **Commit**: Captures repository snapshots
- **Repository**: Manages version control operations

### Design Patterns Used
- Factory Pattern for object creation
- Composition for object relationships
- Singleton for repository management

## Key Features

- Object storage and hashing
- Staging area management
- Commit tracking
- Basic version control operations

## Learning Objectives

1. Understand version control internals
2. Practice advanced C++ design patterns
3. Implement low-level file and object management

## Prerequisites

- C++17 or later
- OpenSSL
- zlib


## Project Status

🚧 Work in Progress
- Basic structure implemented
- Core classes designed
- Advanced features planned

## Roadmap
- [ ] Object storage
- [ ] Commit tracking
- [ ] Branching support
- [ ] Remote repository interactions

## Contributing

Contributions are welcome! Please read the contribution guidelines before submitting a pull request.

## Acknowledgments

Inspired by the Git version control system and built as a learning project.
