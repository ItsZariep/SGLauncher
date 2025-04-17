# SGLauncher
Simple GTK Launcher

[![Packaging status](https://repology.org/badge/vertical-allrepos/sglauncher.svg)](https://repology.org/project/sglauncher/versions)

<details>

<summary><img src="https://translate.codeberg.org/widget/sglauncher/status-badge.png" alt="Status Badge"></summary>

[![Packaging status](https://translate.codeberg.org/widget/sglauncher/multi-green.svg)](https://translate.codeberg.org/engage/sglauncher/)

</details>



## Features:

- [x] Show [system/user apps](https://codeberg.org/ItsZariep/SGLauncher/wiki/Application-detection)
- [x] Search for binaries in `$PATH` 
- [x] Quick access bar
- [x] Search on web button
- [x] Run in terminal button
- [x] [Show calculator answers](https://codeberg.org/ItsZariep/SGLauncher/wiki/Integrated-calculator)
- [x] Show applications actions
- [x] Customizable layout
- [x] Optional icon view
- [x] [Custom CSS styling](https://codeberg.org/ItsZariep/SGLauncher/wiki/Styling)
- [x] Simple configuration tool

 See all available settings in the [documentation](https://itszariep.codeberg.page/projects/sglauncher/wiki.html)

## Build:
- Clone:
```
git clone https://codeberg.org/itszariep/sglauncher
```
```
cd sglauncher/src
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