# SGLauncher
Simple GTK Launcher

## Features:

- [x] Show system menu apps
- [x] Search On Web button
- [x] Run In Terminal button
- [x] Show calc answers
- [x] Show Actions
- [x] Customizable layout
- [x] Optional icon view
- [x] A simple configuration tool

 See all available settings in the [documentation](https://codeberg.org/ItsZariep/SGLauncher/wiki/Settings)

## Build:
- Clone:
```
git clone https://codeberg.org/itszariep/sglauncher
```
```
cd sglauncher
```
- Build
```
make sglauncher
```

- (Optional) Localization files:

```
make compile-locales
```
- (Optional) Install:

```
sudo -E make install
```
> Replace `sudo` if you use an alternative command (like `doas`)


> [!NOTE]  
> The `-E` flag ensures that `sudo` does not reset certain environment variables, which is important because SGLauncher needs to know which version to declare based on the git branch (if it is `stable`, it uses the `VERSION` file; if it is `main`, it uses the `date.commitHash`). This is not needed when running `make` without `sudo` / `doas` / root privileges.