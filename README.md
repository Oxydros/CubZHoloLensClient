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
