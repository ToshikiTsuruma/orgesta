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
 44;
 0.00000;-0.00000;-2.07563;,
 -10.25004;0.00000;0.00000;,
 0.00000;0.90058;-1.87008;,
 -10.25004;0.00000;0.00000;,
 0.00000;1.62279;-1.29413;,
 -10.25004;0.00000;0.00000;,
 0.00000;2.02359;-0.46187;,
 -10.25004;0.00000;0.00000;,
 0.00000;2.02359;0.46187;,
 -10.25004;0.00000;0.00000;,
 0.00000;1.62279;1.29414;,
 -10.25004;0.00000;0.00000;,
 0.00000;0.90058;1.87008;,
 -10.25004;0.00000;0.00000;,
 0.00000;-0.00000;2.07563;,
 -10.25004;0.00000;0.00000;,
 -0.00000;-0.90058;1.87008;,
 -10.25004;0.00000;0.00000;,
 0.00000;-1.62279;1.29413;,
 -10.25004;0.00000;0.00000;,
 0.00000;-2.02359;0.46187;,
 -10.25004;0.00000;0.00000;,
 0.00000;-2.02359;-0.46187;,
 -10.25004;0.00000;0.00000;,
 0.00000;-1.62279;-1.29414;,
 -10.25004;0.00000;0.00000;,
 0.00000;-0.90058;-1.87008;,
 -10.25004;0.00000;0.00000;,
 0.00000;-0.00000;-2.07563;,
 0.00000;-0.00000;-0.00000;,
 0.00000;-0.00000;-2.07563;,
 0.00000;0.90058;-1.87008;,
 0.00000;1.62279;-1.29413;,
 0.00000;2.02359;-0.46187;,
 0.00000;2.02359;0.46187;,
 0.00000;1.62279;1.29414;,
 0.00000;0.90058;1.87008;,
 0.00000;-0.00000;2.07563;,
 -0.00000;-0.90058;1.87008;,
 0.00000;-1.62279;1.29413;,
 0.00000;-2.02359;0.46187;,
 0.00000;-2.02359;-0.46187;,
 0.00000;-1.62279;-1.29414;,
 0.00000;-0.90058;-1.87008;;
 
 28;
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
 3;29,30,31;,
 3;29,31,32;,
 3;29,32,33;,
 3;29,33,34;,
 3;29,34,35;,
 3;29,35,36;,
 3;29,36,37;,
 3;29,37,38;,
 3;29,38,39;,
 3;29,39,40;,
 3;29,40,41;,
 3;29,41,42;,
 3;29,42,43;,
 3;29,43,30;;
 
 MeshMaterialList {
  1;
  28;
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
   0.006400;0.006400;0.006400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  22;
  -0.198472;-0.000000;-0.980107;,
  -0.198472;0.425253;-0.883045;,
  -0.198471;0.766279;-0.611086;,
  -0.198472;0.955533;-0.218094;,
  -0.198472;0.955533;0.218095;,
  -0.198471;0.766278;0.611086;,
  -0.198472;0.425253;0.883045;,
  -0.198472;-0.000000;0.980107;,
  -0.198472;-0.425253;0.883045;,
  -0.198472;-0.766279;0.611086;,
  -0.198472;-0.955533;0.218094;,
  -0.198472;-0.955533;-0.218095;,
  -0.198472;-0.766278;-0.611087;,
  -0.198472;-0.425252;-0.883046;,
  -0.219287;0.762802;-0.608314;,
  1.000000;-0.000000;0.000000;,
  -0.219287;0.423323;0.879039;,
  -0.219287;-0.951199;0.217104;,
  1.000000;0.000000;0.000000;,
  1.000000;-0.000001;-0.000000;,
  1.000000;-0.000000;0.000001;,
  1.000000;0.000001;0.000001;;
  28;
  3;0,14,1;,
  3;1,14,2;,
  3;2,14,3;,
  3;3,14,4;,
  3;4,16,5;,
  3;5,16,6;,
  3;6,16,7;,
  3;7,16,8;,
  3;8,17,9;,
  3;9,17,10;,
  3;10,17,11;,
  3;11,17,12;,
  3;12,13,13;,
  3;13,13,0;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,19;,
  3;15,19,20;,
  3;15,20,21;,
  3;15,21,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;,
  3;15,18,18;;
 }
 MeshTextureCoords {
  44;
  0.000000;1.000000;,
  0.035714;0.000000;,
  0.071429;1.000000;,
  0.071429;0.000000;,
  0.142857;1.000000;,
  0.107143;0.000000;,
  0.214286;1.000000;,
  0.142857;0.000000;,
  0.285714;1.000000;,
  0.178571;0.000000;,
  0.357143;1.000000;,
  0.214286;0.000000;,
  0.428571;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.285714;0.000000;,
  0.571429;1.000000;,
  0.321429;0.000000;,
  0.642857;1.000000;,
  0.357143;0.000000;,
  0.714286;1.000000;,
  0.392857;0.000000;,
  0.785714;1.000000;,
  0.428571;0.000000;,
  0.857143;1.000000;,
  0.464286;0.000000;,
  0.928572;1.000000;,
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
  0.000000;0.000000;;
 }
}
