# MPADT: Muhamix Processor Architecture Detector Tool

MPADT is a lightweight domain-specific language to define architecture-specific build and run rules for your OS kernel or low-level projects.

With MPADT, you describe which assembly or source files to build and run for different CPU architectures in a simple, human-readable syntax, then [MPADT Executor](https://github.com/Muhamix/MPADTE) compiles and runs the correct files automatically based on your system architecture.

---

## Features

- Simple syntax for architecture-specific file mapping  
- Supports multiple rules per architecture  
- Easily extendable for cross-compilation or multi-arch OS projects  
- Integrates assembly, linking, and execution steps automatically

---

## Installation

You don't need to install MPADT to work with it, you just need to install [MPADTE](https://github.com/Muhamix/MPADTE). But, if you want to edit it and add more feature for yourself, clone the repo:

```bash
git clone https://github.com/Muhamix/MPADT.git
```

---

## Usage

Here's some example syntax:

```padl
arch {
    x86 -> "testx86.s"
    arm -> "testarm.s"
    x86 -> "text86_64s.s"
}
```

There can only be one `arch` block, but it can contain as many architecture checks as you need, even of the same architecture.

You can test it with the included `test/` directory:

```bash
# Clone the repo
git clone https://github.com/Muhamix/PADL.git

cd ~/MPADT/tests
../build/mpadt ./test.mpadt
```

---

## Contributions

Contributions are welcome! Please open issues or pull requests for bugs, features, or ideas.

---

## License

[MIT](https://mit-license.org/)
