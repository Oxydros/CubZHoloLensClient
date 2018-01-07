# Preparation of the repo

We are using the git submodule fonctionality to improve our use of internal libraries.

Thus, you have to initialized the repo before using it, by using this commands

```
git submodule sync --recursive
git submodule update --init --remote --merge
```

Now you should have the folder libs/LibNetwork updated and containing all the source code from the master branch.

Launch the bat script initRepo.bat.

You should now see cubznetwork.lib and proto.lib inside the folder libs/bin.

# Useful links

- [Overview about DirectX development on HoloLens](https://developer.microsoft.com/en-us/windows/mixed-reality/directx_development_overview)
- [App model proposed by Microsoft](https://developer.microsoft.com/en-us/windows/mixed-reality/app_model)
- [How to create an holographic DirectX project](https://developer.microsoft.com/en-us/windows/mixed-reality/creating_a_holographic_directx_project)
- [Use PPL library to create parallel tasks](https://msdn.microsoft.com/en-us/library/dd492418.aspx)
- [Coordinate system in Holographic DirectX environment](https://developer.microsoft.com/en-us/windows/mixed-reality/Coordinate_systems_in_DirectX.html)