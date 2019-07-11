import os
import numpy as np
import pysound as ps
from wavefile import WaveWriter, Format
from numpy.random import uniform

def compute_array_shoebox(dim, src_coord, lis_coord, r, s, micarray):
    mesh = ps.createbox(dim[0], dim[1], dim[2], r, s)
    ctx = ps.Context()
    ctx.diffuse_count = 20000
    ctx.specular_count = 2000
    ctx.channel_type = ps.ChannelLayoutType.mono
    scene = ps.Scene()
    scene.setMesh(mesh)

    src = ps.Source(src_coord)
    src.radius = 0.01

    res = {}
    res_buffer = []
    rate = 0
    abandon_flag = False
    for offset in micarray:
        lis = ps.Listener((offset + lis_coord).tolist())
        lis.radius = 0.01

        res_ch = scene.computeIR(src, lis, ctx)
        rate = res_ch['rate']
        sa = res_ch['samples']
        res['rate'] = rate
        res_buffer.append(sa)
    res['samples'] = np.zeros((len(res_buffer), np.max([len(ps) for ps in res_buffer])))
    for i, c in enumerate(res_buffer):
        res['samples'][i, :len(c)] = c
    return res


def main():
    custom_array = np.array(
        [[0, 0.035, 0], [-0.0303, 0.0175, 0], [-0.0303, -0.0175, 0], [0, -0.035, 0], [0.0303, -0.0175, 0],
         [0.0303, 0.0175, 0]])

    nsample = 5
    for isample in range(nsample):
        print('computing room sample {}/{}'.format(isample+1, nsample))
        # sample room dimension and source/listener location
        dim = uniform(1.0, 10.0, 3)
        src_coord = uniform(0.1, 0.9, 3) * dim
        lis_coord = uniform(0.1, 0.9, 3) * dim
        # sample reverb time from normal distribution
        rt = np.clip(np.random.normal(0.7, 0.3), 0.05, 0.95)
        # reverse compute absorption coefficient using empirical formula
        r = np.clip(-0.3345 * np.log(rt) + 0.181, 0, 1)
        # sample scattering coefficient, which has less impact
        s = np.clip(np.random.normal(0.5, 0.25), 0.1, 0.9)

        res = compute_array_shoebox(dim, src_coord, lis_coord, r, s, custom_array)

        w = WaveWriter('room{}.wav'.format(isample), channels=np.shape(res['samples'])[0], samplerate=int(res['rate']))
        w.write(np.array(res['samples']))


if __name__ == '__main__':
    main()

