# Git Commit Guide for Students

A simple guide for writing good git commits in group projects.

## Table of Contents

- [Basic Workflow](#basic-workflow)
- [Commit Message Format](#commit-message-format)
- [Common Commit Types](#common-commit-types)
- [Examples by Situation](#examples-by-situation)
- [Best Practices](#best-practices)
- [Common Commands](#common-commands)
- [Troubleshooting](#troubleshooting)

---

## Basic Workflow

```bash
# 1. Check what changed
git status

# 2. Add files you want to commit
git add <filename>
# or add everything
git add .

# 3. Commit with a message
git commit -m "type: description"

# 4. Push to GitHub
git push
```

---

## Commit Message Format

### Simple Format (One Line)

```
type: Short description of what you did
```

### Detailed Format (Optional)

```
type: Short description

- More details about change 1
- More details about change 2
```

**Rules:**
- Keep the first line under 50 characters
- Start with a lowercase letter after the colon
- Use present tense ("Add" not "Added")
- Be clear and specific

---

## Common Commit Types

| Type | When to Use | Example |
|------|-------------|---------|
| `init:` | First commit, project setup | `init: Initial project setup` |
| `feat:` | New feature or functionality | `feat: Add login page` |
| `fix:` | Bug fixes | `fix: Correct button alignment` |
| `docs:` | Documentation changes | `docs: Update README` |
| `style:` | UI/design changes | `style: Update navbar colors` |
| `refactor:` | Code restructuring | `refactor: Simplify login logic` |
| `chore:` | Build, config, dependencies | `chore: Add CMakeLists.txt` |
| `test:` | Adding or updating tests | `test: Add login validation test` |
| `update:` | General updates | `update: Improve error messages` |
| `remove:` | Removing code/files | `remove: Delete unused images` |

---

## Examples by Situation

### Starting the Project

```bash
git commit -m "init: Initial project setup"
git commit -m "init: Add project structure and files"
```

### Adding Features

```bash
git commit -m "feat: Add user registration form"
git commit -m "feat: Implement account creation"
git commit -m "feat: Add search functionality"
```

### Fixing Bugs

```bash
git commit -m "fix: Correct login validation"
git commit -m "fix: Resolve crash on empty input"
git commit -m "fix: Fix broken navigation links"
```

### Styling/Design

```bash
git commit -m "style: Update homepage layout"
git commit -m "style: Add responsive design"
git commit -m "style: Change color scheme"
```

### Documentation

```bash
git commit -m "docs: Add setup instructions"
git commit -m "docs: Update README"
git commit -m "docs: Add code comments"
```

### Configuration Files

```bash
git commit -m "chore: Add .gitignore"
git commit -m "chore: Update CMakeLists.txt"
git commit -m "chore: Configure build settings"
git commit -m "chore: Add VSCode tasks"
```

### Code Refactoring

```bash
git commit -m "refactor: Simplify database queries"
git commit -m "refactor: Replace wxWidgets with Win32 API"
git commit -m "refactor: Reorganize file structure"
```

### Removing Things

```bash
git commit -m "remove: Delete unused CSS files"
git commit -m "remove: Clean up old test code"
```

### Multiple Related Changes

```bash
git commit -m "chore: Update build config and gitignore"
git commit -m "docs: Update README and USAGE"
```

---

## Best Practices

### ✅ DO

- **Commit often** - Small, focused commits are better
- **Test before committing** - Make sure your code works
- **Pull before push** - Avoid conflicts with teammates
- **Write clear messages** - Your teammates will thank you
- **Commit related changes together** - Build config + gitignore = OK

### ❌ DON'T

- **Don't commit broken code** - Test first!
- **Don't use vague messages** - "fixed stuff", "update", "asdfgh"
- **Don't commit large binary files** - Add them to .gitignore
- **Don't commit unrelated changes together** - New feature + random bug fix = Bad
- **Don't wait too long** - Commit at the end of each work session

### What NOT to Commit

Add these to your `.gitignore`:
```
# Build files
build/
*.exe
*.o

# Libraries
lib/
*.dll
*.so
*.a

# IDE files
.vscode/
.idea/
*.swp

# OS files
.DS_Store
Thumbs.db
```

---

## Common Commands

### Basic Commands

```bash
# Check status
git status

# Add specific file
git add filename.cpp

# Add all changes
git add .

# Commit
git commit -m "type: message"

# Push to GitHub
git push

# Pull latest changes
git pull
```

### Before Starting Work

```bash
# Always pull first to get teammates' changes
git pull
```

### After Finishing Work

```bash
# Check what changed
git status

# Add and commit
git add .
git commit -m "feat: Add your feature"

# Push
git push
```

### Fixing Last Commit Message

```bash
# If you haven't pushed yet
git commit --amend -m "type: corrected message"
```

### Viewing History

```bash
# See recent commits
git log --oneline

# See last 5 commits
git log --oneline -5
```

### Undoing Changes

```bash
# Undo changes to a file (before git add)
git restore filename.cpp

# Unstage a file (after git add, before commit)
git restore --staged filename.cpp

# Undo last commit (keep changes)
git reset --soft HEAD~1
```

---

## Troubleshooting

### "Please commit your changes or stash them before you merge"

```bash
# Save your work
git add .
git commit -m "your message"

# Then pull
git pull
```

### "failed to push some refs"

```bash
# Pull first, then push
git pull
git push
```

### Large File Error (over 100MB)

```bash
# Add to .gitignore
echo "path/to/large/file" >> .gitignore

# Remove from git (keeps file on computer)
git rm --cached path/to/large/file

# Commit and push
git add .gitignore
git commit -m "chore: Remove large files from tracking"
git push
```

### Merge Conflicts

```bash
# 1. Pull changes
git pull

# 2. Open conflicted files and fix them
# Look for <<<<<<< HEAD markers

# 3. After fixing
git add .
git commit -m "fix: Resolve merge conflicts"
git push
```

### Accidentally Committed to Wrong Branch

```bash
# Create new branch from current state
git branch correct-branch

# Go back to main
git checkout main

# Remove the commit from main
git reset --hard HEAD~1

# Switch to correct branch
git checkout correct-branch
```

---

## Quick Reference Card

**Starting work:**
```bash
git pull
```

**After making changes:**
```bash
git status
git add .
git commit -m "type: what you did"
git push
```

**Common types:**
- `init:` - First time
- `feat:` - New stuff
- `fix:` - Fixing bugs
- `docs:` - Documentation
- `style:` - Design/UI
- `chore:` - Config/build
- `refactor:` - Code cleanup

---

## For Group Projects

### Communication Tips

1. **Tell your team before making big changes**
2. **Pull before you start working**
3. **Push when you finish a feature**
4. **Write clear commit messages** - they'll read them!
5. **Don't force push** unless everyone agrees

### Avoiding Conflicts

- Pull frequently (start of day, before pushing)
- Commit and push often
- Communicate who's working on what
- Don't edit the same files at the same time

---

## Need Help?

- Check git status: `git status`
- View recent commits: `git log --oneline`
- Ask your teammates!
- Search: "git [your problem]"

---

**Remember: Good commits = Happy teammates! 🚀**
