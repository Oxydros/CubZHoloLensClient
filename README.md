# Repo initialization

We are using the git submodule fonctionality to improve our use of internal libraries.

Thus, you have to initialized the repo before using it, by using this commands

```
git submodule sync --recursive
git submodule update --init --remote --merge
```

Now you should have the folder libs/LibNetwork updated and containing all the source code from the master branch.

The LibNetwork project uses the Protobuf and the Boost libraries. Please refer to the [readme](https://gitlab.com/cubz/LibNetworks/blob/master/README.md) to view the instructions.

When everything is in order, launch the bat script initRepo.bat. It should compile the LibNetworks withour errors.

You should now see cubznetwork.lib and proto.lib inside the folder libs/bin.

# Useful links

- [Overview about DirectX development on HoloLens](https://developer.microsoft.com/en-us/windows/mixed-reality/directx_development_overview)
- [App model proposed by Microsoft](https://developer.microsoft.com/en-us/windows/mixed-reality/app_model)
- [How to create an holographic DirectX project](https://developer.microsoft.com/en-us/windows/mixed-reality/creating_a_holographic_directx_project)
- [Use PPL library to create parallel tasks](https://msdn.microsoft.com/en-us/library/dd492418.aspx)
- [Coordinate system in Holographic DirectX environment](https://developer.microsoft.com/en-us/windows/mixed-reality/Coordinate_systems_in_DirectX.html)
- [Stability of the holograms](https://developer.microsoft.com/en-us/windows/mixed-reality/hologram_stability)
- [Hologram collision](https://forums.hololens.com/discussion/6785/understand-when-a-user-clicked-gaze-on-particular-object-in-directx-newbie)
- [Other link for collision](https://forums.hololens.com/discussion/8185/how-to-identify-if-a-hologram-is-in-user-gaze-view-in-directx-uwp-c-hololens-app)
- [Gesture and motion inputs](https://developer.microsoft.com/en-us/windows/mixed-reality/gaze,_gestures,_and_motion_controllers_in_directx)
- [Lightning and texture in Dx11][https://www.3dgep.com/texturing-lighting-directx-11/]

# Library used

- [DirectX Tool Kit](https://github.com/Microsoft/DirectXTK)
- [DirectX Mesh](https://github.com/Microsoft/DirectXMesh)
- [DirectX Texture](https://github.com/Microsoft/DirectXTex)