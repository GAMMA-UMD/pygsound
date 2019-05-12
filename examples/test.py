import numpy as np
import pysound as ps
from wavefile import WaveWriter, Format


def main():
    mesh = ps.loadobj("cube.obj", "", 0.5, 0.5)
    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.stereo
    scene = ps.Scene()
    scene.setMesh(mesh)

    src_coord = [1, 1, 0.5]
    lis_coord = [5, 3, 0.5]

    src = ps.Source(src_coord)
    src.radius = 0.01

    lis = ps.Listener(lis_coord)
    lis.radius = 0.01

    res = scene.computeMultichannelIR(src, lis, ctx)

    w = WaveWriter('test1.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate']))
    w.write(np.array(res['samples']))

    mesh2 = ps.createbox(10, 6, 2, 0.5, 0.5)
    scene = ps.Scene()
    scene.setMesh(mesh2)

    res2 = scene.computeMultichannelIR(src, lis, ctx)
    w2 = WaveWriter('test2.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate']))
    w2.write(np.array(res2['samples']))


if __name__ == '__main__':
    main()