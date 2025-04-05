// Imports
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// Game States
typedef enum { SPLASH, MENU, RANDOMIZER } GameState;
GameState state = SPLASH;

// Fonts and Textures
Font valorantFont, descriptionFont;
Texture2D bgImage, faceImage, menuBackground;

// Role Selection
const char *roles[] = {"ALL", "DUELIST", "CONTROLLER", "INITIATOR", "SENTINEL"};
Rectangle roleButtons[5]; 
const char *selectedRole = "ALL"; 

// Button Tweening configuration ( increases with effect until it hits 1 which is transition compleate )
float buttonLerp[5] = {0, 0, 0, 0, 0}; 
float backButtonLerp = 0.0f;

// Smoothly transitions from 'start' to 'end' based on 'amount' (0.0 to 1.0).
float LerpFloat(float start, float end, float amount) {
    return start + amount * (end - start);
}
// Agent Struct
typedef struct {
    const char *name;
    const char *role;
    const char *abilities[4];
    const char *bgPath;
    const char *facePath;
    const char *description;
} Agent;


// sets up the agents and their abilities, backgrounds, and faces as an array of structs

Agent agents[] = {
    {"Jett", "DUELIST", {"Cloudburst", "Updraft", "Tailwind", "Blade Storm"}, "resources/jett_bg.png", "resources/jett_face.png",
    "Representing her home country of South Korea, Jett's agile and evasive fighting style lets her take risks no one else can. She runs circles around every skirmish, cutting enemies before they even know what hit them."},

   {"Clove", "CONTROLLER", {"Meddle", "Pick-me-up", "Ruse", "Not Dead Yet"}, "resources/clove_bg.png", "resources/clove_face.png",
    "Scottish troublemaker Clove makes mischief for enemies in both the heat of combat and the cold of death. The young immortal keeps foes guessing, even from beyond the grave, their return to the living only ever a moment away."},

   {"Sage", "SENTINEL", {"Barrier Orb", "Slow Orb", "Healing Orb", "Resurrection"}, "resources/sage_bg.png", "resources/sage_face.png",
    "The bastion of China, Sage creates safety for herself and her team wherever she goes. Able to revive fallen friends and stave off aggressive pushes, she provides a calm center to a hellish fight."},

   {"Breach", "INITIATOR", {"Aftershock", "Flashpoint", "Fault Line", "Rolling Thunder"}, "resources/breach_bg.png", "resources/breach_face.png",
    "Breach, the bionic Swede, fires powerful, targeted kinetic blasts to aggressively clear a path through enemy ground. The damage and disruption he inflicts ensures no fight is ever fair."},

   {"Cypher", "SENTINEL", {"Trapwire", "Cyber Cage", "Spycam", "Neural Theft"}, "resources/cypher_bg.png", "resources/cypher_face.png",
    "The Moroccan information broker, Cypher is a one-man surveillance network who keeps tabs on the enemy’s every move. No secret is safe. No maneuver goes unseen. Cypher is always watching."},

   {"Killjoy", "SENTINEL", {"Nanoswarm", "Alarmbot", "Turret", "Lockdown"}, "resources/killjoy_bg.png", "resources/killjoy_face.png",
    "The genius of Germany, Killjoy effortlessly secures key battlefield positions with her arsenal of inventions. If their damage doesn't take her enemies out, the debuff her robots provide will make short work of them."},

   {"Omen", "CONTROLLER", {"Shrouded Step", "Paranoia", "Dark Cover", "From the Shadows"}, "resources/omen_bg.png", "resources/omen_face.png",
    "A phantom of a memory, Omen hunts in the shadows. He renders enemies blind, teleports across the field, then lets paranoia take hold as his foe scrambles to learn where he might strike next."},

   {"Phoenix", "DUELIST", {"Blaze", "Curveball", "Hot Hands", "Run It Back"}, "resources/phoenix_bg.png", "resources/phoenix_face.png",
    "Hailing from the U.K., Phoenix's star power shines through in his fighting style, igniting the battlefield with flash and flare. Whether he's got backup or not, he'll rush into a fight on his own terms."},

   {"Raze", "DUELIST", {"Boom Bot", "Paint Shells", "Blast Pack", "Showstopper"}, "resources/raze_bg.png", "resources/raze_face.png",
    "Raze explodes out of Brazil with her big personality and big guns. With her blunt-force-trauma playstyle, she excels at flushing entrenched enemies and clearing tight spaces with a generous dose of “boom.”"},

   {"Reyna", "DUELIST", {"Leer", "Devour", "Dismiss", "Empress"}, "resources/reyna_bg.png", "resources/reyna_face.png",
    "Forged in the heart of Mexico, Reyna dominates single combat, popping off with each kill she scores. Her capability is only limited by her raw skill, making her highly dependent on performance."},

   {"Sova", "INITIATOR", {"Owl Drone", "Shock Bolt", "Recon Bolt", "Hunter's Fury"}, "resources/sova_bg.png", "resources/sova_face.png",
    "Born from the eternal winter of Russia's tundra, Sova tracks, finds, and eliminates enemies with ruthless efficiency and precision. His custom bow and incredible scouting abilities ensure that even if you run, you cannot hide."},

    {"Viper", "CONTROLLER", {"Snake Bite", "Poison Cloud", "Toxic Screen", "Viper's Pit"}, "resources/viper_bg.png", "resources/viper_face.png",
    "The American chemist, Viper deploys an array of poisonous chemical devices to control the battlefield and choke the enemy's vision. If the toxins don't kill her prey, her mind games surely will."},

   {"Yoru", "DUELIST", {"Fakeout", "Blindside", "Gatecrash", "Dimensional Drift"}, "resources/yoru_bg.png", "resources/yoru_face.png",
    "Japanese native Yoru rips holes straight through reality to infiltrate enemy lines unseen. Using deception and aggression in equal measure, he gets the drop on each target before they know where to look."},

   {"Brimstone", "CONTROLLER", {"Incendiary", "Stim Beacon", "Sky Smoke", "Orbital Strike"}, "resources/brimstone_bg.png", "resources/brimstone_face.png",
    "Joining from the U.S.A., Brimstone's orbital arsenal ensures his squad always has the advantage. His ability to deliver utility precisely and from a distance make him an unmatched boots-on-the-ground commander."},

   {"Skye", "INITIATOR", {"Regrowth", "Trailblazer", "Guiding Light", "Seekers"}, "resources/skye_bg.png", "resources/skye_face.png",
    "Hailing from Australia, Skye and her band of beasts trail-blaze the way through hostile territory. With her creations hampering the enemy, and her power to heal others, the team is strongest and safest by Skye's side."},

   {"Astra", "CONTROLLER", {"Gravity Well", "Nova Pulse", "Nebula", "Cosmic Divide"}, "resources/astra_bg.png", "resources/astra_face.png",
    "Ghanaian Agent Astra harnesses the energies of the cosmos to reshape battlefields to her whim. With full command of her astral form and a talent for deep strategic foresight, s"},

   {"KAY/O", "INITIATOR", {"FRAG/ment", "FLASH/drive", "ZERO/point", "NULL/cmd"}, "resources/kayo_bg.png", "resources/kayo_face.png",
    "KAY/O is a machine of war built for a single purpose: neutralizing radiants. His power to suppress enemy abilities dismantles his opponents' capacity to fight back, securing him and his allies the ultimate edge."},

   {"Chamber", "SENTINEL", {"Trademark", "Headhunter", "Rendezvous", "Tour de Force"}, "resources/chamber_bg.png", "resources/chamber_face.png",
    "Well-dressed and well-armed, French weapons designer Chamber expels aggressors with deadly precision. He leverages his custom arsenal to hold the line and pick off enemies from afar, with a contingency built for every plan."},

   {"Neon", "DUELIST", {"High Gear", "Relay Bolt", "Fast Lane", "Overdrive"}, "resources/neon_bg.png", "resources/neon_face.png",
    "Filipino Agent, Neon, surges forward at shocking speeds, discharging bursts of bioelectric radiance as fast as her body generates it. She races ahead to catch enemies off guard, then strikes them down quicker than lightning."},

   {"Fade", "INITIATOR", {"Prowler", "Seize", "Haunt", "Nightfall"}, "resources/fade_bg.png", "resources/fade_face.png",
    "Turkish bounty hunter Fade unleashes the power of raw nightmare to seize enemy secrets. Attuned with terror itself, she hunts down targets and reveals their deepest fears - before crushing them in the dark."},

   {"Harbor", "CONTROLLER", {"Cascade", "Cove", "High Tide", "Reckoning"}, "resources/harbor_bg.png", "resources/harbor_face.png",
    "Hailing from India's coast, Harbor storms the field wielding ancient technology with dominion over water. He unleashes frothing rapids and crushing waves to shield his allies, or pummel those that oppose him."},

   {"Gekko", "INITIATOR", {"Dizzy", "Wingman", "Mosh Pit", "Thrash"}, "resources/gekko_bg.png", "resources/gekko_face.png",
    "Gekko the Angeleno leads a tight-knit crew of calamitous creatures. His buddies bound forward, scattering enemies out of the way, with Gekko chasing them down to regroup and go again."},

   {"Deadlock", "SENTINEL", {"GravNet", "Sonic Sensor", "Barrier Mesh", "Annihilation"}, "resources/deadlock_bg.png", "resources/deadlock_face.png",
    "Norwegian operative Deadlock deploys an array of cutting-edge nanowire to secure the battlefield from even the most lethal assault. No one escapes her vigilant watch, nor survives her unyielding ferocity."},

   {"Iso", "DUELIST", {"Undercut", "Contingency", "Double Tap", "Kill Contract"}, "resources/iso_bg.png", "resources/iso_face.png",
    "Chinese fixer for hire Iso goes into a flow state to dismantle the opposition. Reconfiguring ambient energy into bulletproof protection, he advances with focus towards his next duel to the death."},

   {"Vyse", "SENTINEL", {"Shear", "Razorvine", "Arc Rose", "Steel Garden"}, "resources/vyse_bg.png", "resources/vyse_face.png",
    "Metallic mastermind Vyse unleashes liquid metal to isolate, trap, and disarm her enemies. Through cunning and manipulation, she forces all who oppose her to fear the battlefield itself."},

   {"Tejo", "INITIATOR", {"Stealth Drone", "Special Delivery", "Guided Salvo", "Armageddon"}, "resources/tejo_bg.png", "resources/tejo_face.png",
    "A veteran intelligence consultant from Colombia, Tejo’s ballistic guidance system pressures the enemy to relinquish their ground - or their lives. His targeted strikes keep opponents off balance and under his heel."}
};

// sets the agent count to the number of agents in the array

int agentCount = sizeof(agents) / sizeof(agents[0]);

// sets the selected agent to the default agent

Agent selectedAgent;

// loads the assets 

void LoadAssets() {
    valorantFont = LoadFont("resources/Valorant Font.ttf");
    descriptionFont = LoadFontEx("resources/Conthrax-SemiBold.otf", 32, NULL, 0);
    bgImage = LoadTexture("default_bg.png"); // fallback incase an agent background is not found
    faceImage = LoadTexture("default_face.png"); // fallback incase an agent image is not found
    menuBackground = LoadTexture("resources/menu.png");  
}

// loads the agent images for the app based on selection and randomization of agents 

void LoadAgentImages(const char *bgPath, const char *facePath) {
    if (bgImage.id != 0) UnloadTexture(bgImage);
    if (faceImage.id != 0) UnloadTexture(faceImage);

    bgImage = LoadTexture(bgPath);
    faceImage = LoadTexture(facePath);
}

// gets the agent by role and filters the agents based on the role selected

Agent GetAgentByRole(const char *roleFilter) {
    Agent filteredAgents[agentCount]; // temporary array to store filtered agents
    int filteredCount = 0; // count of filtered agents
    for (int i = 0; i < agentCount; i++) { // loop through all agents
        if (strcmp(roleFilter, "ALL") == 0 || strcmp(agents[i].role, roleFilter) == 0) { // check if agent role matches the filter
            filteredAgents[filteredCount++] = agents[i]; // add agent to filtered array
        }
    }

    if (filteredCount == 0) { // if no agents found, return default agent
        return (Agent){"No Agent Found", "N/A", {"N/A", "N/A", "N/A", "N/A"}, "default_bg.png", "default_face.png"}; // default agent
    }

    int randomIndex = rand() % filteredCount; // get random index from filtered agents
    LoadAgentImages(filteredAgents[randomIndex].bgPath, filteredAgents[randomIndex].facePath); // load agent images
    return filteredAgents[randomIndex]; //  return random agent
}

// Splash Screen

// Splash Screen
float startButtonLerp = 0.0f; // Lerp value for button animation

void DrawSplashScreen() {
    DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, 
                   (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0.0f, WHITE);

    Vector2 valorantSize = MeasureTextEx(valorantFont, "VALORANT", 60, 2);
    Vector2 randomizerSize = MeasureTextEx(valorantFont, "RANDOMIZER", 60, 2);

    float valorantX = (GetScreenWidth() - valorantSize.x) / 2;
    float randomizerX = (GetScreenWidth() - randomizerSize.x) / 2;

    DrawTextEx(valorantFont, "VALORANT", (Vector2){ valorantX, 180 }, 60, 2, RED);
    DrawTextEx(valorantFont, "RANDOMIZER", (Vector2){ randomizerX, 250 }, 60, 2, RED);

    // Start Button
    Rectangle startButton = { 250, 450, 300, 80 };

    // Measure text size
    Vector2 textSize = MeasureTextEx(valorantFont, "SELECT ROLE", 30, 2);
    float textX = startButton.x + (startButton.width - textSize.x) / 2;
    float textY = startButton.y + (startButton.height - textSize.y) / 2;

 
    if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
        startButtonLerp += 0.06f;  // configures the speed of the fade in
        if (startButtonLerp > 1.0f) startButtonLerp = 1.0f;
    } else {
        startButtonLerp -= 0.06f;  // configures the speed of the fade out
        if (startButtonLerp < 0.0f) startButtonLerp = 0.0f;
    }

// uses tweening or lerping to blend button color from RED to DARKGRAY based on hover (0.0 to 1.0).

        Color buttonColor = (Color){
        (int)LerpFloat(RED.r, DARKGRAY.r, startButtonLerp),
        (int)LerpFloat(RED.g, DARKGRAY.g, startButtonLerp),
        (int)LerpFloat(RED.b, DARKGRAY.b, startButtonLerp),
        255
    };

    Color textColor = (Color){
        (int)LerpFloat(WHITE.r, RED.r, startButtonLerp),
        (int)LerpFloat(WHITE.g, RED.g, startButtonLerp),
        (int)LerpFloat(WHITE.b, RED.b, startButtonLerp),
        255
    };

    // Draw the button
    DrawRectangleRounded(startButton, 1.0f, 12, buttonColor);
    DrawTextEx(valorantFont, "SELECT ROLE", (Vector2){ textX, textY }, 30, 2, textColor);

    // Button Click -> Switch to Menu
    if (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = MENU;
    }
}

// Draw Role Selection
void DrawRoleSelection() {
    DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, 
                   (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0.0f, WHITE);
    
    Vector2 titleSize = MeasureTextEx(valorantFont, "SELECT AN AGENT ROLE", 40, 2);
    DrawTextEx(valorantFont, "SELECT AN AGENT ROLE", (Vector2){(GetScreenWidth() - titleSize.x) / 2, 100}, 40, 2, RED);
    
    Vector2 mousePos = GetMousePosition();

// uses tweening or lerping to blend button color from RED to DARKGRAY based on hover (0.0 to 1.0).

    for (int i = 0; i < 5; i++) {
        roleButtons[i] = (Rectangle){ 200, 200 + (i * 80), 400, 60 };
        if (CheckCollisionPointRec(mousePos, roleButtons[i])) {
            buttonLerp[i] += 0.06f;
            if (buttonLerp[i] > 1.0f) buttonLerp[i] = 1.0f;
        } else {
            buttonLerp[i] -= 0.06f;
            if (buttonLerp[i] < 0.0f) buttonLerp[i] = 0.0f;
        }

        Color buttonColor = (Color){ 
            (int)LerpFloat(RED.r, DARKGRAY.r, buttonLerp[i]),
            (int)LerpFloat(RED.g, DARKGRAY.g, buttonLerp[i]),
            (int)LerpFloat(RED.b, DARKGRAY.b, buttonLerp[i]),
            255
        };

        DrawRectangleRounded(roleButtons[i], 1.0f, 12, buttonColor);
        Vector2 textSize = MeasureTextEx(valorantFont, roles[i], 30, 2);
        float textX = roleButtons[i].x + (roleButtons[i].width - textSize.x) / 2;
        float textY = roleButtons[i].y + (roleButtons[i].height - textSize.y) / 2;
        DrawTextEx(valorantFont, roles[i], (Vector2){ textX, textY }, 30, 2, WHITE);

        if (CheckCollisionPointRec(mousePos, roleButtons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedRole = roles[i];
            selectedAgent = GetAgentByRole(selectedRole);
            state = RANDOMIZER;
        }
    }
}

// Draw Wrapped Text for agent descriptions

void DrawWrappedText(const char *text, Font font, Rectangle rec, float fontSize, float spacing, Color color) {
    int len = strlen(text);
    float y = rec.y;
    float x = rec.x;
    float maxWidth = rec.width;
    float wordWidth = 0;
    float spaceWidth = MeasureTextEx(font, " ", fontSize, spacing).x;

    char word[128] = {0};
    int wordIndex = 0;

    for (int i = 0; i <= len; i++) {
        if (text[i] == ' ' || text[i] == '\0') {
            word[wordIndex] = '\0';
            wordWidth = MeasureTextEx(font, word, fontSize, spacing).x;

            if (x + wordWidth > rec.x + maxWidth) {
                x = rec.x;
                y += fontSize + spacing;
            }

            DrawTextEx(font, word, (Vector2){ x, y }, fontSize, spacing, color);
            x += wordWidth + spaceWidth;
            wordIndex = 0;

        } else {
            word[wordIndex++] = text[i];
        }
    }
}

// Agent Screen

void DrawAgentScreen() {
    DrawTexturePro(menuBackground, (Rectangle){0, 0, menuBackground.width, menuBackground.height}, // draws the background image
    (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()}, (Vector2){0, 0}, 0.0f, WHITE); // sets the background image to the screen size

    float scale = fmin((float)GetScreenWidth() / bgImage.width, (float)GetScreenHeight() / bgImage.height); // scales the image
    int newWidth = bgImage.width * scale; // sets the new width for the image
    int newHeight = bgImage.height * scale; // sets the new height for the image
    int offsetX = (GetScreenWidth() - newWidth) / 2; // centers the image
    int offsetY = (GetScreenHeight() - newHeight) / 2; // centers the image

    DrawTexturePro(bgImage, (Rectangle){0, 0, bgImage.width, bgImage.height}, // draws the background image
    (Rectangle){offsetX, offsetY, newWidth, newHeight}, (Vector2){0, 0}, 0.0f, WHITE); // centers the image
    DrawTextEx(valorantFont, selectedAgent.name, (Vector2){ 50, 50 }, 50, 2, LIGHTGRAY); // draws the agent name
    DrawTextEx(valorantFont, selectedAgent.role, (Vector2){ 50, 120 }, 30, 2, RED); // draws the agent role

    // Description Box

    Rectangle descriptionBox = {50, 180, 500, 120}; 
    DrawWrappedText(selectedAgent.description,  descriptionFont, descriptionBox, 20, 2, WHITE);

    // Agent info box (abilities)

    Rectangle infoBox = {50, 550, 500, 200}; 
    DrawRectangleRec(infoBox, Fade(LIGHTGRAY, 0.3f));

    // Put abilities inside the info box

    for (int i = 0; i < 4; i++) {
        char abilityText[50];
        sprintf(abilityText, " Ability %d - %s", i + 1, selectedAgent.abilities[i]);
        DrawTextEx(descriptionFont, abilityText, (Vector2){ 100, 580 + (i * 40) }, 25, 2, WHITE);
    }

    // Face image positioning
    int faceSize = 200;
    int faceX = 550;
    int faceY = 550;

    DrawRectangleRec((Rectangle){faceX, faceY, faceSize, faceSize}, Fade(LIGHTGRAY, 0.3f));
    DrawTexturePro(faceImage, (Rectangle){0, 0, faceImage.width, faceImage.height}, 
                   (Rectangle){faceX, faceY, faceSize, faceSize}, (Vector2){0, 0}, 0.0f, WHITE);


//  face box position to align the back button with the edge of the face box

int faceXbtn = 550;  
int faceWidthbtn = 200; 
int backButtonY = 50; 

// Back button 
Vector2 textSize = MeasureTextEx(descriptionFont, "< Back", 30, 2);
Rectangle backButton = { (faceXbtn + faceWidthbtn) - (textSize.x + 20), backButtonY, textSize.x + 20, 40 };

// Tweening 
if (CheckCollisionPointRec(GetMousePosition(), backButton)) {
    backButtonLerp += 0.06f;  // Slower fade-in
    if (backButtonLerp > 1.0f) backButtonLerp = 1.0f;
} else {
    backButtonLerp -= 0.06f;  // Slower fade-out
    if (backButtonLerp < 0.0f) backButtonLerp = 0.0f;
}

// set colors using LerpFloat()
Color buttonColor = (Color){
    (int)LerpFloat(RED.r, DARKGRAY.r, backButtonLerp),
    (int)LerpFloat(RED.g, DARKGRAY.g, backButtonLerp),
    (int)LerpFloat(RED.b, DARKGRAY.b, backButtonLerp),
    255
};

Color textColor = (Color){
    (int)LerpFloat(WHITE.r, RED.r, backButtonLerp),
    (int)LerpFloat(WHITE.g, RED.g, backButtonLerp),
    (int)LerpFloat(WHITE.b, RED.b, backButtonLerp),
    255
};

// Draw back button with animation
DrawRectangleRounded(backButton, 1.0f, 12, buttonColor);
DrawTextEx(descriptionFont, "< Back", (Vector2){ backButton.x + 10, backButton.y + 5 }, 30, 2, textColor);

// Check if button is clicked
if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    state = MENU;
}
}


int main() {
    srand(time(NULL));
    InitWindow(800, 800, "Valorant Agent Randomizer");
    InitAudioDevice();
    LoadAssets();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        switch (state) {
            case SPLASH: DrawSplashScreen(); break;
            case MENU: DrawRoleSelection(); break;
            case RANDOMIZER: DrawAgentScreen(); break;
        }

        EndDrawing();
    }

    UnloadTexture(bgImage);
    UnloadTexture(faceImage);
    UnloadFont(valorantFont);
    UnloadFont(descriptionFont);
    UnloadTexture(menuBackground);
    CloseWindow();
    
    return 0; // need to return something so returning 0
}