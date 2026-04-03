## GiveMiiUplay
A plugin designed to allow the "Uplay" Wii U app to function again.
## How does it work?
We currently only hook act::AcquireIndependentServiceToken to satisfy Uplay, However a DNS redirect is required to redirect to a custom [Web Server](https://github.com/GuestDreemurr/Kasane) and [Quazal Rendez-vous Server](https://github.com/GuestDreemurr/uplay-wiiu-rdv).



## Credits
[GiveMiiYoutube](https://github.com/PretendoNetwork/GiveMiiYouTube) For providing a basic template for skipping service token check, all credits go to people mentioned in the README of that project.


## Building
```bash
# Build docker image (only needed once)
docker build . -t givemiiuplay_builder

# make 
docker run -it --rm -v ${PWD}:/project givemiiuplay_builder make

# make clean
docker run -it --rm -v ${PWD}:/project givemiiuplay_builder make clean
```



