
// -----------------------------------------------------------------
// vr_glove - a VR glove made with common components and recycled stuff
// Copyright (C) 2023  Gabriele Bonacini
//
// This program is distributed under dual license:
// - Creative Comons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) License 
// for non commercial use, the license has the following terms:
// * Attribution — You must give appropriate credit, provide a link to the license, 
// and indicate if changes were made. You may do so in any reasonable manner, 
// but not in any way that suggests the licensor endorses you or your use.
// * NonCommercial — You must not use the material for commercial purposes.
// * NonAI - You must not to use the material to instruct AI / Machine learning systems.
// A copy of the license it's available to the following address:
// http://creativecommons.org/licenses/by-nc/4.0/
// For commercial use a specific license is available contacting the author.
// -----------------------------------------------------------------


#include <glove.h>

glove::Glove* gl = nullptr;

void setup(void) {
  gl = new glove::Glove();
}

void loop() {
    gl->handleEvents();
    gl->sendMsg();
    delay(100);  
}
