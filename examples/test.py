import os
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

    src_coord = [1, 1, 1]
    lis_coord = [0, 0, 0]

    src = ps.Source(src_coord)
    src.radius = 0.01

    lis = ps.Listener(lis_coord)
    lis.radius = 0.01

    res = scene.computeMultichannelIR(src, lis, ctx)

    w = WaveWriter('test.wav', channels=np.shape(res['samples'])[0], samplerate=int(res['rate']))
    w.write(np.array(res['samples']))


if __name__ == '__main__':
    main()