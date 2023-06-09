# BossDeathCounterInternal

Program to easily keep track of deaths in various games, including adding bosses and keeping track of time spent on bosses\
Shows overlay in corners of the game that displays current deaths and current boss


# Run

Inject `BossDeathCounterInternalDX11x64.dll`(64bit) or `BossDeathCounterInternalDX11x86.dll`(32bit) using any injector supporting LoadLibrary(unload functionality) into desired game


# Usage
Use menu to operate on program

`insert` - open menu\
`F1` - uninject program\
`F3` - increment deaths\
`F9` - decrement deaths

# Problems
If u cant inject dll make sure that dll u are using is the same architecture as the game\
Inject `BossDeathCounterInternalDX11x64.dll` dll if game is 64-bit\
Inject `BossDeathCounterInternalDX11x86.dll` dll if game is 32-bit

# Images
![alt text](https://i.ibb.co/qs4xbYP/obraz-2023-06-09-050648912.png)

# Sources
* [Source](https://www.unknowncheats.me/forum/d3d-tutorials-and-source/457178-imgui-hook-directx12-directx11-directx9-x64-x86.html) - Based on this source
* [ImGui](https://github.com/ocornut/imgui) - Gui Library
* [ImGui-Notify](https://github.com/patrickcjk/imgui-notify) - Imgui notficications
* [Kiero](https://github.com/Rebzzel/kiero) - Graphics hooking
* [MinHook](https://github.com/TsudaKageyu/minhook) - Library hooking
