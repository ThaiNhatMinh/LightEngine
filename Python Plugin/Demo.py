
pFile = open("AK47.LTB","rb")
print ("Name of the file: ", pFile.name)
print ("Closed or not : ", pFile.closed)
print ("Opening mode : ", pFile.mode)

import struct

#Header = pFile.read(16)
print (struct.unpack("BHBIII",pFile.read(20)))
print (struct.unpack("IIIIIIIIIIIIIIII",pFile.read(16*4)))

#comand name
strlen = 0
struct.unpack("H",pFile.read(2))
print(strlen)
#m_GlobalRadius
print (struct.unpack("f",pFile.read(4)))
#m_iNumEnabledOBBs
print (struct.unpack("I",pFile.read(4)))

#numPieces
numPieces = struct.unpack("I",pFile.read(4))[0]

while numPieces>0:
    
   
        
    
        
    numPieces = numPieces-1





class ModelPiece:
    
    def __init__(self):
        self.name = "unknow"
    def Load(self,pFile):
        
        strlen = struct.unpack("H",pFile.read(2))[0]
        self.name = pFile.read(strlen)
        self.m_nLODs = struct.unpack("I",pFile.read(4))[0]
        temp = self.m_nLODs
        while temp>0:
            m_pLODDists[temp] = struct.unpack("f",pFile.read(4))[0]
            temp = temp - 1

        pFile.read(8)
        temp = self.m_nLODs
        while temp>0:
            self.m_nNumTextures = struct.unpack("I",pFile.read(4))[0]
            for i in range(4):
                self.m_iTextures[i] = struct.unpack("i",pFile.read(4))[0]

            self.m_iRenderStyle = struct.unpack("i",pFile.read(4))[0]
            self.m_nRenderPriority = struct.unpack("B",pFile.read(1))[0]

            render_object_type = struct.unpack("I",pFile.read(4))[0]
            iObjSize = struct.unpack("I",pFile.read(4))[0]
            self.iVertCount = struct.unpack("I",pFile.read(4))[0]
            self.iPolyCount = struct.unpack("I",pFile.read(4))[0]
            self.iMaxBonesPerTri = struct.unpack("I",pFile.read(4))[0]
            self.iMaxBonesPerVert = struct.unpack("I",pFile.read(4))[0]
            self.bReIndexBones = struct.unpack("c",pFile.read(1))[0]
            self.StreamData[0] = struct.unpack("I",pFile.read(4))[0]
            self.StreamData[1] = struct.unpack("I",pFile.read(4))[0]
            self.StreamData[2] = struct.unpack("I",pFile.read(4))[0]
            self.StreamData[3] = struct.unpack("I",pFile.read(4))[0]
            UseMatrixPalettes = struct.unpack("c",pFile.read(1))[0]

            for i in range(4):
                if StreamData[i]>0:
                    
