import meshio
import sys
import os

def convertMesh():
    filepath = sys.argv[1]
    extension = sys.argv[2]
    dir = os.path.dirname(filepath) + "/"
    output_filepath = dir + ((os.path.basename(filepath)).split(sep='.'))[0] + "." + extension
    mesh = meshio.read(filepath)
    mesh.write(output_filepath)

convertMesh()
