# Coup: Object-Oriented Strategy Game

### Created by: **Amit Nachum**

**Email:** [nachum.amit@msmail.ariel.ac.il](mailto:nachum.amit@msmail.ariel.ac.il)

---

## ✨ Project Description

This project is a full-featured, object-oriented simulation of the strategic party game **Coup**, implemented in C++. It includes:

* Turn-based role gameplay with 6 unique roles
* Rich rule enforcement: coup costs, turn validation, blocking
* Fully interactive **GUI** using **SFML**
* AI players with strategy injection via the Strategy design pattern
* Factory pattern for flexible player creation
* Unit testing using **Doctest**

---

## ⚙ Requirements

* C++20 compiler (e.g. g++)
* [SFML 2.5+](https://www.sfml-dev.org/)
* Doctest for testing
* Linux environment recommended (tested on Ubuntu)

---

## 🔧 Compilation & Usage

Run one of the following **make commands**:

### `make gui`  ✨

> **(Must be run before other targets)**

* Compiles the main GUI program.
* Output: `./gui`

### `make test`

* Compiles and runs all unit tests (`test_coup.cpp`) using Doctest.
* Output: test results printed to terminal.

### `make valgrind`

* Runs the GUI with **Valgrind** to detect memory leaks.
* ⚠️ Note: SFML may generate **false positives** in Valgrind due to internal allocations.

### `make clean`

* Deletes all compiled object files and binaries.

---

## 🌟 Features

* ☑️ 6 roles: **Governor**, **Baron**, **General**, **Spy**, **Judge**, **Merchant**
* ☑️ Each role has unique mechanics: `prevent_coup`, `invest`, `block_tax`, `view_pile`, `cancel_extra_turn`, etc.
* ☑️ Game logic enforces:

  * Turn order
  * Blockable actions
  * Coup at 10+ coins
  * Sanctions and bribes
* ☑️ Undo support
* ☑️ Persistent arrest tracking
* ☑️ GUI shows player states, coins, and role buttons dynamically
* ☑️ Audio integration support (via SFML's audio module)

---

## ⚖️ Testing

* Tests cover all roles and game mechanics
* Includes invalid moves, blocked actions, coin limits, player death, and special abilities
* Format:

```cpp
CHECK_NOTHROW(role->tax());
CHECK_THROWS(role->coup(*self));
CHECK(role->get_coins() == expected);
```

---

## ⚡ Notes

* Be sure to call `make gui` **first** before running tests or Valgrind.
* All player memory is managed by the `Game` class.
* Use `background.ogg` and `background.png` to add ambiance.

---

## 🚀 Future Ideas

* Multiplayer over sockets
* Dynamic player loadout with config files
* Persistent match replay logs

---

**Made with passion and precision** 

> "A Coup well-planned is a Coup well-won."

---
