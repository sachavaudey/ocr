

/*
char* image_input[nbTest][nbInput]={
        "images_test/AA.PNG",
        "images_test/BB.PNG",
        "images_test/CC.PNG",
        "images_test/DD.PNG",
        "images_test/EE.PNG",
        "images_test/FF.PNG",
        "images_test/GG.PNG",
        "images_test/HH.PNG",
        "images_test/II.PNG",
        "images_test/JJ.PNG",
        "images_test/KK.PNG",
        "images_test/LL.PNG",
        "images_test/MM.PNG",
        "images_test/NN.PNG",
        "images_test/OO.PNG",
        "images_test/PP.PNG",


    };


    // Liste des images à traiter
    char* images[nbTest] = {
        "images_test/A.PNG",
        "images_test/B.PNG",
        "images_test/C.PNG",
        "images_test/D.PNG",
        "images_test/E.PNG",
        "images_test/F.PNG",
        "images_test/G.PNG",
        "images_test/H.PNG",
        "images_test/I.PNG",
        "images_test/J.PNG",
        "images_test/K.PNG",
        "images_test/L.PNG",
        "images_test/M.PNG",
        "images_test/N.PNG",
        "images_test/O.PNG",
        "images_test/P.PNG",

    };


        double training_output[nbTest][output] = {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                  {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                  {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
                                                  {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
                                                  {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
                                                  {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
                                                  {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
                                                  {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
                                                  {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
                                                  {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
                                                  {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
                                                  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
                                                  {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
                                                  {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                                                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                                                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
                                                


        };*/











/*

ALPHABET ENTIER

char* image_input[nbTest][nbInput]={
        "images_test/AA.PNG",
        "images_test/BB.PNG",
        "images_test/CC.PNG",
        "images_test/DD.PNG",
        "images_test/EE.PNG",
        "images_test/FF.PNG",
        "images_test/GG.PNG",
        "images_test/HH.PNG",
        "images_test/II.PNG",
        "images_test/JJ.PNG",
        "images_test/KK.PNG",
        "images_test/LL.PNG",
        "images_test/MM.PNG",
        "images_test/NN.PNG",
        "images_test/OO.PNG",
        "images_test/PP.PNG",
        "images_test/QQ.PNG",
        "images_test/RR.PNG",
        "images_test/SS.PNG",
        "images_test/TT.PNG",
        "images_test/UU.PNG",
        "images_test/VV.PNG",
        "images_test/WW.PNG",
        "images_test/XX.PNG",
        "images_test/YY.PNG",
        "images_test/ZZ.PNG",


    };


    // Liste des images à traiter
    char* images[nbTest] = {
        "images_test/A.PNG",
        "images_test/B.PNG",
        "images_test/C.PNG",
        "images_test/D.PNG",
        "images_test/E.PNG",
        "images_test/F.PNG",
        "images_test/G.PNG",
        "images_test/H.PNG",
        "images_test/I.PNG",
        "images_test/J.PNG",
        "images_test/K.PNG",
        "images_test/L.PNG",
        "images_test/M.PNG",
        "images_test/N.PNG",
        "images_test/O.PNG",
        "images_test/P.PNG",
        "images_test/Q.PNG",
        "images_test/R.PNG",
        "images_test/S.PNG",
        "images_test/T.PNG",
        "images_test/U.PNG",
        "images_test/V.PNG",
        "images_test/W.PNG",
        "images_test/X.PNG",
        "images_test/Y.PNG",
        "images_test/Z.PNG",

    };


        double training_output[nbTest][output] = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // A
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // B
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // C
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // D
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // E
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // F
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // G
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // H
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // I
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // J
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // K
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // L
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, // M
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, // N
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0}, // O
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0}, // P
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0}, // Q
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0}, // R
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0}, // S
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0}, // T
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0}, // U
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0}, // V
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0}, // W
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0}, // X
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}, // Y
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}  // Z
};





