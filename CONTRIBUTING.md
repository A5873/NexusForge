# Contributing to NexusForge

Thank you for your interest in contributing to NexusForge! This document outlines the process for contributing to the project and the guidelines that contributors should follow.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [How to Contribute](#how-to-contribute)
- [Development Workflow](#development-workflow)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)
- [Documentation](#documentation)
- [Community](#community)

## Code of Conduct

NexusForge is committed to fostering an open and welcoming environment. Please read and follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## Getting Started

1. **Fork** the repository on GitHub
2. **Clone** your fork to your local machine
3. **Create a branch** for your feature or bug fix
4. **Set up** the development environment following the instructions in the README

## How to Contribute

There are many ways to contribute to NexusForge:

- **Code contributions**: Implement new features or fix bugs
- **Documentation**: Improve or extend documentation
- **Examples**: Create example projects showcasing capabilities
- **Testing**: Help identify and fix issues
- **Ideas**: Suggest new features or improvements

### Contribution Ideas

Not sure where to start? Here are some ideas:

- Check out the [issues](https://github.com/nexusforge/nexusforge/issues) labeled "good first issue"
- Improve existing documentation
- Add unit tests
- Optimize performance
- Add comments to complex code sections

## Development Workflow

NexusForge follows a Git-based workflow:

1. The `main` branch contains the stable, production-ready code
2. All development occurs in feature branches
3. Changes are integrated via pull requests with required reviews
4. Continuous integration runs automated testing on all pull requests

## Coding Standards

Please follow the coding standards outlined in our [Engineering Strategy](docs/design/ENGINEERING_STRATEGY.md). Key points include:

- Use 4 spaces for indentation (no tabs)
- Maximum line length of 100 characters
- Follow the naming conventions in the engineering strategy
- Write clear, concise comments
- Keep functions small and focused
- Write tests for your code

## Commit Guidelines

- Write clear, concise commit messages
- Start with a verb in the present tense (e.g., "Add", "Fix", "Update")
- Reference issue numbers when relevant
- Keep commits focused on single changes
- Make frequent, smaller commits rather than large, sweeping changes

Example:
```
Fix crash when loading large textures

- Add error checking in texture loader
- Implement fallback mechanism for out-of-memory conditions
- Update documentation with memory requirements

Fixes #123
```

## Pull Request Process

1. Ensure your code meets the coding standards
2. Update documentation to reflect any changes
3. Add tests for new features or bug fixes
4. Ensure all tests pass locally
5. Submit a pull request with a clear description of the changes
6. Address any feedback from reviewers

## Documentation

Good documentation is crucial for NexusForge's success:

- Document all public APIs
- Keep the README up-to-date
- Create tutorials for new features
- Update architecture documents when making significant changes
- Add inline comments for complex code sections

## Community

Join the NexusForge community:

- Star and watch the repository
- Join discussions in issues and pull requests
- Help answer questions from other users
- Share your projects built with NexusForge

Thank you for contributing to NexusForge! Your efforts help make this project better for everyone.

