import numpy as np
import pygsound as ps
from wavefile import WaveWriter, Format


def main():
    # Simulation using .obj file (and an optional .mtl file)
    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.stereo
    
    mesh1 = ps.loadobj("cube.obj", "")    # if the second argument is empty, the code will infer the .mtl name using .obj name
    scene = ps.Scene()
    scene.setMesh(mesh1)

    src_coord = [1, 1, 0.5]
    lis_coord = [5, 3, 0.5]

    src = ps.Source(src_coord)
    src.radius = 0.01

    lis = ps.Listener(lis_coord)
    lis.radius = 0.01

    res = scene.computeMultichannelIR(src, lis, ctx)

    with WaveWriter('test1.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate'])) as w1:
        w1.write(np.array(res['samples']))
        print("IR using .obj input written to test1.wav.")

    # Simulation using a shoebox definition
    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.stereo

    mesh2 = ps.createbox(10, 6, 2, 0.5, 0.5)
    scene = ps.Scene()
    scene.setMesh(mesh2)

    res = scene.computeMultichannelIR(src, lis, ctx)
    with WaveWriter('test2.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate'])) as w:
        w2.write(np.array(res['samples']))
        print("IR using shoebox input written to test2.wav.")


if __name__ == '__main__':
    main()
