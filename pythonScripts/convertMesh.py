import meshio
import sys

def convertMesh():
    file = sys.argv[1]
    fileNoExt = file.split(sep='.')[0]
    mesh = meshio.read("/home/bill/projects/libmesh-skinning/Meshes/" + file)
    mesh.write("/home/bill/projects/libmesh-skinning/Meshes/" + fileNoExt + ".off")

convertMesh()
