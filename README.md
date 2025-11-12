# ⚓ BATTLESHIP: THE DIVINE CONQUEST

> *Let the holy ships guide your fate!*

A suckless implementation of the classic Battleship game, written in pure C with divine inspiration. Command your sacred fleet across holy waters in this terminal-based naval warfare experience.

## ✨ Features

- **Pure C Implementation** - No dependencies, just raw C and the standard library
- **5x5 Sacred Grid** - Compact battlefield for quick, strategic gameplay
- **3 Ship Classes** - Patrol Boat (1), Destroyer (2), and Battleship (3)
- **Divine Guidance** - Atmospheric terminal interface with spiritual flair
- **Smart AI Opponent** - Computer strategically places and fires at your fleet
- **Input Validation** - Robust error handling for all player actions

## 🎮 How to Play

### Compilation

```bash
gcc -o battleship battleship.c
```

### Running the Game

```bash
./battleship
```

### Game Rules

1. **Ship Placement Phase**
   - Place your three ships on the 5x5 grid
   - Enter coordinates like `A0`, `B3`, etc.
   - Choose horizontal (0) or vertical (1) orientation
   - Ships cannot overlap or go off the grid

2. **Battle Phase**
   - Take turns firing at enemy coordinates
   - `X` marks a hit, `O` marks a miss
   - Sink all enemy ships to achieve victory
   - Defend your fleet from the computer's attacks

3. **Victory Conditions**
   - First player to sink all opponent ships wins
   - May God's light shine upon your missiles!

## 🎯 Game Symbols

| Symbol | Meaning |
|--------|---------|
| `.` | Empty water |
| `#` | Your ship (placement phase) |
| `X` | Hit |
| `O` | Miss |
| `?` | Unknown (enemy waters) |

## 🏛️ The Suckless Philosophy

This implementation follows suckless principles:

- **Minimal** - Under 250 lines of clean C code
- **Efficient** - No unnecessary abstractions or bloat
- **Portable** - Runs anywhere with a C compiler
- **Readable** - Clear logic, simple functions, no magic

## 🔧 Technical Details

- Grid size: 5x5
- Ships: 3 (lengths 1, 2, 3)
- Language: C (C99 compatible)
- Dependencies: Standard library only (`stdio.h`, `stdlib.h`, `time.h`, `ctype.h`)
- Platform: Cross-platform (Linux, macOS, Windows with MinGW)

## 📜 License

This is free and unencumbered software released into the public domain.

## 🙏 Divine Inspiration

*"In the sacred waters of the 5x5 grid, only the righteous shall prevail."*

---

**May your aim be true and your fleet unsinkable!** ⚓
