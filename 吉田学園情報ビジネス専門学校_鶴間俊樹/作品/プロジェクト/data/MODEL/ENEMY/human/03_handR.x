xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 50;
 0.00000;0.00000;-2.18600;,
 -19.23561;0.00000;-0.00031;,
 0.00000;0.83644;-2.01963;,
 -19.23561;0.00000;-0.00031;,
 0.00000;1.54553;-1.54583;,
 -19.23561;0.00000;-0.00031;,
 0.00000;2.01931;-0.83674;,
 -19.23561;0.00000;-0.00031;,
 0.00000;2.18568;-0.00031;,
 -19.23561;0.00000;-0.00031;,
 0.00000;2.01931;0.83612;,
 -19.23561;0.00000;-0.00031;,
 0.00000;1.54553;1.54520;,
 -19.23561;0.00000;-0.00031;,
 0.00000;0.83644;2.01900;,
 -19.23561;0.00000;-0.00031;,
 0.00000;0.00000;2.18538;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-0.83645;2.01900;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-1.54553;1.54520;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-2.01931;0.83612;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-2.18568;-0.00031;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-2.01931;-0.83674;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-1.54553;-1.54583;,
 -19.23561;0.00000;-0.00031;,
 0.00000;-0.83645;-2.01963;,
 -19.23561;0.00000;-0.00031;,
 0.00000;0.00000;-2.18600;,
 0.00000;0.00000;-0.00031;,
 0.00000;0.00000;-2.18600;,
 0.00000;0.83644;-2.01963;,
 0.00000;1.54553;-1.54583;,
 0.00000;2.01931;-0.83674;,
 0.00000;2.18568;-0.00031;,
 0.00000;2.01931;0.83612;,
 0.00000;1.54553;1.54520;,
 0.00000;0.83644;2.01900;,
 0.00000;0.00000;2.18538;,
 0.00000;-0.83645;2.01900;,
 0.00000;-1.54553;1.54520;,
 0.00000;-2.01931;0.83612;,
 0.00000;-2.18568;-0.00031;,
 0.00000;-2.01931;-0.83674;,
 0.00000;-1.54553;-1.54583;,
 0.00000;-0.83645;-2.01963;;
 
 32;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;16,17,18;,
 3;18,19,20;,
 3;20,21,22;,
 3;22,23,24;,
 3;24,25,26;,
 3;26,27,28;,
 3;28,29,30;,
 3;30,31,32;,
 3;33,34,35;,
 3;33,35,36;,
 3;33,36,37;,
 3;33,37,38;,
 3;33,38,39;,
 3;33,39,40;,
 3;33,40,41;,
 3;33,41,42;,
 3;33,42,43;,
 3;33,43,44;,
 3;33,44,45;,
 3;33,45,46;,
 3;33,46,47;,
 3;33,47,48;,
 3;33,48,49;,
 3;33,49,34;;
 
 MeshMaterialList {
  1;
  32;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.200000;0.200000;0.200000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  21;
  -0.112901;0.000001;-0.993606;,
  -0.112901;0.380237;-0.917972;,
  -0.112901;0.702592;-0.702579;,
  -0.112901;0.917977;-0.380225;,
  -0.112900;0.993606;-0.000000;,
  -0.112901;0.917977;0.380225;,
  -0.112901;0.702592;0.702580;,
  -0.112901;0.380237;0.917972;,
  -0.112901;0.000001;0.993606;,
  -0.112901;-0.380238;0.917972;,
  -0.112901;-0.702593;0.702578;,
  -0.112901;-0.917977;0.380225;,
  -0.112900;-0.993606;0.000000;,
  -0.112901;-0.917977;-0.380225;,
  -0.112901;-0.702593;-0.702578;,
  -0.112901;-0.380238;-0.917972;,
  -0.129639;0.824455;-0.550879;,
  1.000000;0.000000;0.000000;,
  -0.129640;0.193446;0.972508;,
  -0.129638;-0.972509;-0.193442;,
  -0.110758;-0.193883;-0.974752;;
  32;
  3;0,16,1;,
  3;1,16,2;,
  3;2,16,3;,
  3;3,16,4;,
  3;4,16,5;,
  3;5,18,6;,
  3;6,18,7;,
  3;7,18,8;,
  3;8,18,9;,
  3;9,18,10;,
  3;10,19,11;,
  3;11,19,12;,
  3;12,19,13;,
  3;13,19,14;,
  3;14,19,15;,
  3;15,20,0;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;,
  3;17,17,17;;
 }
 MeshTextureCoords {
  50;
  0.000000;1.000000;,
  0.031250;0.000000;,
  0.062500;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.093750;0.000000;,
  0.187500;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.156250;0.000000;,
  0.312500;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.218750;0.000000;,
  0.437500;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.281250;0.000000;,
  0.562500;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.343750;0.000000;,
  0.687500;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.406250;0.000000;,
  0.812500;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.468750;0.000000;,
  0.937500;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
