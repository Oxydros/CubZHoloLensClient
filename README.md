# Preparation of the repo

We are using the git submodule fonctionality to improve our use of internal libraries.

Thus, you have to initialized the repo before using it, by using this commands

```git submodule init```

```git submodule update```

Now you should have the folder libs/LibNetwork updated and containing all the source code from the master branch.

Launch the bat script GenerateVSSolution.bat to compile this source code.

You should now see cubznetwork.lib and proto.lib inside the folder libs/bin.
