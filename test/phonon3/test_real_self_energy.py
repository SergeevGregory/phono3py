"""Test for real_self_energy.py."""

import numpy as np

from phono3py import Phono3py
from phono3py.phonon3.real_self_energy import ImagToReal


def test_real_self_energy_with_band_indices(si_pbesol: Phono3py):
    """Real part of self energy spectrum of Si.

    * at frequencies of band indices.

    """
    if si_pbesol._make_r0_average:
        ref_Delta = [
            [
                -0.0057326,
                -0.0057326,
                -0.01633157,
                -0.14797815,
                -0.15086179,
                -0.15086179,
            ],
            [
                -0.02080177,
                -0.02104276,
                -0.06575259,
                -0.11439863,
                -0.13666415,
                -0.14372152,
            ],
        ]
    else:
        ref_Delta = [
            [
                -0.0057666,
                -0.0057666,
                -0.01639729,
                -0.14809965,
                -0.15091765,
                -0.15091765,
            ],
            [
                -0.02078728,
                -0.02102094,
                -0.06573269,
                -0.11432603,
                -0.1366966,
                -0.14371315,
            ],
        ]
    si_pbesol.mesh_numbers = [9, 9, 9]
    si_pbesol.init_phph_interaction()
    _, delta = si_pbesol.run_real_self_energy(
        si_pbesol.grid.grg2bzg[[1, 103]],
        [
            300,
        ],
        write_hdf5=False,
        frequency_points_at_bands=True,
    )
    # print(delta[0, 0, :])
    np.testing.assert_allclose(ref_Delta, delta[0, 0, :], atol=0.01)


def test_real_self_energy_with_frequency_points(si_pbesol: Phono3py):
    """Real part of self energy spectrum of Si.

    * specified frquency points

    """
    if si_pbesol._make_r0_average:
        ref_Delta_fps = [
            [
                -0.00573260,
                -0.00589756,
                -0.00839449,
                -0.00966402,
                -0.00573260,
                -0.00589756,
                -0.00839449,
                -0.00966402,
                -0.01486569,
                -0.01633157,
                -0.01993995,
                -0.02073417,
                -0.15512961,
                -0.14758526,
                -0.14797816,
                -0.14222444,
                -0.15683457,
                -0.15691468,
                -0.15974324,
                -0.15086181,
                -0.15683457,
                -0.15691468,
                -0.15974324,
                -0.15086181,
            ],
            [
                -0.01990876,
                -0.02077841,
                -0.01798683,
                -0.01933049,
                -0.02158770,
                -0.02193100,
                -0.02186381,
                -0.01877096,
                -0.05743552,
                -0.05244391,
                -0.06241623,
                -0.05643933,
                -0.13068294,
                -0.11925247,
                -0.13470590,
                -0.13105466,
                -0.15192994,
                -0.14204618,
                -0.14364976,
                -0.14173656,
                -0.14762541,
                -0.13904288,
                -0.14284911,
                -0.14112034,
            ],
        ]
    else:
        ref_Delta_fps = [
            [
                -0.00576660,
                -0.00594616,
                -0.00840087,
                -0.00960344,
                -0.00576660,
                -0.00594616,
                -0.00840087,
                -0.00960344,
                -0.01493508,
                -0.01639729,
                -0.01997820,
                -0.02070427,
                -0.15511645,
                -0.14747203,
                -0.14809966,
                -0.14230763,
                -0.15674925,
                -0.15684992,
                -0.15983868,
                -0.15091767,
                -0.15674925,
                -0.15684992,
                -0.15983868,
                -0.15091767,
            ],
            [
                -0.01990306,
                -0.02077094,
                -0.01798066,
                -0.01935581,
                -0.02158076,
                -0.02190634,
                -0.02195633,
                -0.01882258,
                -0.05740055,
                -0.05240406,
                -0.06252644,
                -0.05651015,
                -0.13072273,
                -0.11929265,
                -0.13472599,
                -0.13105120,
                -0.15191900,
                -0.14202698,
                -0.14371246,
                -0.14168892,
                -0.14760248,
                -0.13907618,
                -0.14275290,
                -0.14100562,
            ],
        ]

    si_pbesol.mesh_numbers = [9, 9, 9]
    si_pbesol.init_phph_interaction()
    frequency_points = [1.469947, 3.085309, 14.997187, 15.129080]
    fps, delta = si_pbesol.run_real_self_energy(
        si_pbesol.grid.grg2bzg[[1, 103]],
        [
            300,
        ],
        frequency_points=frequency_points,
        write_hdf5=False,
        frequency_points_at_bands=False,
    )

    # for b in delta[0, 0, 0]:
    #     print("".join(f"{s:.8f}, " % s for s in b))
    # for b in delta[0, 0, 1]:
    #     print("".join(f"{s:.8f}, " % s for s in b))

    np.testing.assert_allclose(frequency_points, fps, atol=1e-5)
    np.testing.assert_allclose(ref_Delta_fps[0], delta[0, 0, 0].ravel(), atol=0.01)
    np.testing.assert_allclose(ref_Delta_fps[1], delta[0, 0, 1].ravel(), atol=0.01)


def test_real_self_energy_nacl_nac_npoints(nacl_pbe: Phono3py):
    """Real part of self energy spectrum of NaCl.

    * at 10 frequency points sampled uniformly.
    * at q->0

    """
    ref_freq_points_nacl_nac = [
        0.0,
        1.63223063,
        3.26446125,
        4.89669188,
        6.5289225,
        8.16115313,
        9.79338375,
        11.42561438,
        13.057845,
        14.69007563,
    ]

    if nacl_pbe._make_r0_average:
        ref_delta_nacl_nac = [
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                -0.43955299,
                -0.24659961,
                -0.45227602,
                -0.80296691,
                -0.35433156,
                0.54514663,
                0.29417714,
                0.16346531,
                0.11216578,
            ],
            [
                0.00000000,
                -0.43955299,
                -0.24659961,
                -0.45227602,
                -0.80296691,
                -0.35433156,
                0.54514663,
                0.29417714,
                0.16346531,
                0.11216578,
            ],
            [
                0.00000000,
                -0.27249573,
                -0.15287654,
                -0.28038322,
                -0.49778992,
                -0.21966368,
                0.33795727,
                0.18237167,
                0.10133841,
                0.06953586,
            ],
        ]
    else:
        ref_delta_nacl_nac = [
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
                0.00000000,
            ],
            [
                0.00000000,
                -0.43835860,
                -0.24732790,
                -0.45125795,
                -0.80781808,
                -0.36217377,
                0.54848873,
                0.29335955,
                0.16329652,
                0.11208513,
            ],
            [
                0.00000000,
                -0.43835860,
                -0.24732790,
                -0.45125795,
                -0.80781808,
                -0.36217377,
                0.54848873,
                0.29335955,
                0.16329652,
                0.11208513,
            ],
            [
                0.00000000,
                -0.27175528,
                -0.15332803,
                -0.27975208,
                -0.50079735,
                -0.22452538,
                0.34002916,
                0.18186482,
                0.10123376,
                0.06948586,
            ],
        ]

    nacl_pbe.mesh_numbers = [9, 9, 9]
    nacl_pbe.init_phph_interaction(nac_q_direction=[1, 0, 0])
    fps, delta = nacl_pbe.run_real_self_energy(
        [nacl_pbe.grid.gp_Gamma], [300], num_frequency_points=10
    )

    # for line in delta[0, 0, 0]:
    #     print("[", ",".join([f"{val:.8f}" for val in line]), "],")

    np.testing.assert_allclose(ref_freq_points_nacl_nac, fps.ravel(), rtol=0, atol=1e-5)
    np.testing.assert_allclose(ref_delta_nacl_nac, delta[0, 0, 0], rtol=0, atol=1e-2)


def test_ImagToReal():
    """Test ImagToReal class (Kramers–Kronig relation)."""
    # imag-self-energy Si-PBEsol 50x50x50 gp=5, bi=4, 101 points, 300K
    im_part = [
        [0.0000000, 0.0000000, -0.0000000, -0.1750223, 0.1534611, -0.1794583],
        [0.3069222, 0.0180686, 0.3069222, -0.1692049, 0.4603833, -0.1636592],
        [0.6138444, 0.0158916, 0.6138444, -0.1621245, 0.7673055, -0.1625514],
        [0.9207667, 0.0238034, 0.9207667, -0.1556618, 1.0742278, -0.1484696],
        [1.2276889, 0.0131519, 1.2276889, -0.1515719, 1.3811500, -0.1525501],
        [1.5346111, 0.0124129, 1.5346111, -0.1541357, 1.6880722, -0.1566124],
        [1.8415333, 0.0165506, 1.8415333, -0.1556995, 1.9949944, -0.1567434],
        [2.1484555, 0.0196960, 2.1484555, -0.1556492, 2.3019166, -0.1563791],
        [2.4553778, 0.0225276, 2.4553778, -0.1553117, 2.6088389, -0.1562531],
        [2.7623000, 0.0269193, 2.7623000, -0.1536529, 2.9157611, -0.1520174],
        [3.0692222, 0.0247502, 3.0692222, -0.1530976, 3.2226833, -0.1545476],
        [3.3761444, 0.0253261, 3.3761444, -0.1565178, 3.5296055, -0.1617560],
        [3.6830666, 0.0360216, 3.6830666, -0.1586552, 3.8365277, -0.1618262],
        [3.9899888, 0.0510642, 3.9899888, -0.1523272, 4.1434499, -0.1463620],
        [4.2969111, 0.0494221, 4.2969111, -0.1428688, 4.4503722, -0.1388442],
        [4.6038333, 0.0474328, 4.6038333, -0.1361604, 4.7572944, -0.1307086],
        [4.9107555, 0.0370451, 4.9107555, -0.1345154, 5.0642166, -0.1359581],
        [5.2176777, 0.0375397, 5.2176777, -0.1361887, 5.3711388, -0.1356714],
        [5.5245999, 0.0345936, 5.5245999, -0.1377925, 5.6780610, -0.1390885],
        [5.8315222, 0.0327132, 5.8315222, -0.1425343, 5.9849833, -0.1472336],
        [6.1384444, 0.0374592, 6.1384444, -0.1489307, 6.2919055, -0.1562694],
        [6.4453666, 0.0555985, 6.4453666, -0.1481088, 6.5988277, -0.1452657],
        [6.7522888, 0.0586765, 6.7522888, -0.1409779, 6.9057499, -0.1375563],
        [7.0592110, 0.0560470, 7.0592110, -0.1371741, 7.2126721, -0.1365273],
        [7.3661333, 0.0550400, 7.3661333, -0.1372613, 7.5195944, -0.1387434],
        [7.6730555, 0.0565263, 7.6730555, -0.1399341, 7.8265166, -0.1440837],
        [7.9799777, 0.0624044, 7.9799777, -0.1457614, 8.1334388, -0.1582010],
        [8.2868999, 0.1009214, 8.2868999, -0.1369623, 8.4403610, -0.1261905],
        [8.5938221, 0.1090238, 8.5938221, -0.1087372, 8.7472832, -0.0870371],
        [8.9007444, 0.0807253, 8.9007444, -0.0903838, 9.0542055, -0.0835210],
        [9.2076666, 0.0658383, 9.2076666, -0.0875470, 9.3611277, -0.0853012],
        [9.5145888, 0.0592222, 9.5145888, -0.0867791, 9.6680499, -0.0838069],
        [9.8215110, 0.0521071, 9.8215110, -0.0853895, 9.9749721, -0.0820186],
        [10.1284332, 0.0419588, 10.1284332, -0.0862870, 10.2818943, -0.0860513],
        [10.4353554, 0.0364097, 10.4353554, -0.0892629, 10.5888165, -0.0892228],
        [10.7422777, 0.0316193, 10.7422777, -0.0922975, 10.8957388, -0.0926032],
        [11.0491999, 0.0274798, 11.0491999, -0.0955924, 11.2026610, -0.0960711],
        [11.3561221, 0.0233126, 11.3561221, -0.0994131, 11.5095832, -0.1004271],
        [11.6630443, 0.0193464, 11.6630443, -0.1043388, 11.8165054, -0.1065527],
        [11.9699665, 0.0174292, 11.9699665, -0.1099907, 12.1234276, -0.1125414],
        [12.2768888, 0.0164239, 12.2768888, -0.1157951, 12.4303499, -0.1188108],
        [12.5838110, 0.0165308, 12.5838110, -0.1218120, 12.7372721, -0.1253937],
        [12.8907332, 0.0184723, 12.8907332, -0.1275275, 13.0441943, -0.1309579],
        [13.1976554, 0.0211215, 13.1976554, -0.1325454, 13.3511165, -0.1358727],
        [13.5045776, 0.0244407, 13.5045776, -0.1369047, 13.6580387, -0.1400194],
        [13.8114999, 0.0282908, 13.8114999, -0.1404205, 13.9649610, -0.1429165],
        [14.1184221, 0.0313175, 14.1184221, -0.1435582, 14.2718832, -0.1462968],
        [14.4253443, 0.0348581, 14.4253443, -0.1467576, 14.5788054, -0.1495649],
        [14.7322665, 0.0386150, 14.7322665, -0.1500845, 14.8857276, -0.1533339],
        [15.0391887, 0.0424503, 15.0391887, -0.1550110, 15.1926498, -0.1625259],
        [15.3461109, 0.0605747, 15.3461109, -0.1550584, 15.4995720, -0.1547951],
        [15.6530332, 0.0727408, 15.6530332, -0.1443151, 15.8064943, -0.1346934],
        [15.9599554, 0.0625829, 15.9599554, -0.1346277, 16.1134165, -0.1305515],
        [16.2668776, 0.0547742, 16.2668776, -0.1324223, 16.4203387, -0.1301107],
        [16.5737998, 0.0459500, 16.5737998, -0.1348752, 16.7272609, -0.1365713],
        [16.8807220, 0.0432888, 16.8807220, -0.1397155, 17.0341831, -0.1410867],
        [17.1876443, 0.0398939, 17.1876443, -0.1450244, 17.3411054, -0.1471968],
        [17.4945665, 0.0351088, 17.4945665, -0.1540409, 17.6480276, -0.1617873],
        [17.8014887, 0.0424833, 17.8014887, -0.1635035, 17.9549498, -0.1699703],
        [18.1084109, 0.0556675, 18.1084109, -0.1649880, 18.2618720, -0.1626782],
        [18.4153331, 0.0532241, 18.4153331, -0.1631579, 18.5687942, -0.1618524],
        [18.7222554, 0.0458460, 18.7222554, -0.1672846, 18.8757165, -0.1709147],
        [19.0291776, 0.0443150, 19.0291776, -0.1758322, 19.1826387, -0.1808661],
        [19.3360998, 0.0454289, 19.3360998, -0.1853104, 19.4895609, -0.1911626],
        [19.6430220, 0.0483353, 19.6430220, -0.1952856, 19.7964831, -0.2019949],
        [19.9499442, 0.0534799, 19.9499442, -0.2053378, 20.1034053, -0.2123304],
        [20.2568665, 0.0599861, 20.2568665, -0.2151359, 20.4103276, -0.2225551],
        [20.5637887, 0.0685856, 20.5637887, -0.2240731, 20.7172498, -0.2305568],
        [20.8707109, 0.0755373, 20.8707109, -0.2331629, 21.0241720, -0.2419628],
        [21.1776331, 0.0906438, 21.1776331, -0.2384003, 21.3310942, -0.2380604],
        [21.4845553, 0.0827018, 21.4845553, -0.2476712, 21.6380164, -0.2593767],
        [21.7914775, 0.0907297, 21.7914775, -0.2660044, 21.9449386, -0.2793854],
        [22.0983998, 0.1025946, 22.0983998, -0.2859215, 22.2518609, -0.3026711],
        [22.4053220, 0.1210133, 22.4053220, -0.3084041, 22.5587831, -0.3297156],
        [22.7122442, 0.1517773, 22.7122442, -0.3307280, 22.8657053, -0.3535969],
        [23.0191664, 0.1918115, 23.0191664, -0.3505455, 23.1726275, -0.3769971],
        [23.3260886, 0.2484881, 23.3260886, -0.3659809, 23.4795497, -0.3980745],
        [23.6330109, 0.3449959, 23.6330109, -0.3587633, 23.7864720, -0.3785512],
        [23.9399331, 0.4932745, 23.9399331, -0.2683146, 24.0933942, -0.1740930],
        [24.2468553, 0.4000672, 24.2468553, -0.1714061, 24.4003164, -0.1388929],
        [24.5537775, 0.3476085, 24.5537775, -0.1478397, 24.7072386, -0.1341295],
        [24.8606997, 0.3031722, 24.8606997, -0.1588841, 25.0141608, -0.1796324],
        [25.1676220, 0.3332258, 25.1676220, -0.1716723, 25.3210831, -0.1741573],
        [25.4745442, 0.3514490, 25.4745442, -0.1621265, 25.6280053, -0.1553650],
        [25.7814664, 0.3514801, 25.7814664, -0.1503569, 25.9349275, -0.1458311],
        [26.0883886, 0.3490583, 26.0883886, -0.1437619, 26.2418497, -0.1387991],
        [26.3953108, 0.3346525, 26.3953108, -0.1471754, 26.5487719, -0.1497006],
        [26.7022331, 0.3145614, 26.7022331, -0.1719482, 26.8556942, -0.1898755],
        [27.0091553, 0.2913173, 27.0091553, -0.2370443, 27.1626164, -0.2939869],
        [27.3160775, 0.2707732, 27.3160775, -0.4139632, 27.4695386, -0.6910918],
        [27.6229997, 0.9436577, 27.6229997, -0.3688420, 27.7764608, -0.2026185],
        [27.9299219, 0.9147269, 27.9299219, -0.1095194, 28.0833830, -0.0365244],
        [28.2368441, 0.9515446, 28.2368441, 0.0642006, 28.3903052, 0.1596622],
        [28.5437664, 0.9296466, 28.5437664, 0.2357042, 28.6972275, 0.3228099],
        [28.8506886, 0.9383991, 28.8506886, 0.4390380, 29.0041497, 0.6142345],
        [29.1576108, 0.7465231, 29.1576108, 0.6502426, 29.3110719, 0.8357126],
        [29.4645330, 0.3472854, 29.4645330, 0.6927159, 29.6179941, 0.6873352],
        [29.7714552, 0.2258269, 29.7714552, 0.6147208, 29.9249163, 0.6117702],
        [30.0783775, 0.1397870, 30.0783775, 0.5578788, 30.2318386, 0.5591731],
        [30.3852997, 0.0468188, 30.3852997, 0.4941608, 30.5387608, 0.4706733],
        [30.6922219, 0.0000000, 30.6922219, 0.4173657, 30.8456830, 0.3821416],
    ]

    vals = np.array(im_part)
    i2r = ImagToReal(vals[:, 1], vals[:, 0])
    i2r.run()
    pick_one_vals = -1 * i2r.re_part  # -1 to make it freq-shift
    pick_one_freqs = i2r.frequency_points
    i2r.run(method="half_shift")
    half_shift_vals = -1 * i2r.re_part  # -1 to make it freq-shift
    half_shift_freqs = i2r.frequency_points
    # for f, im, f1, re1, f2, re2, in zip(
    #         vals[:, 0], vals[:, 1],
    #         pick_one_freqs, pick_one_vals,
    #         half_shift_freqs, half_shift_vals):
    #     print("[%.7f, %.7f, %.7f, %.7f, %.7f, %.7f],"
    #           % (f, im, f1, re1, f2, re2))
    np.testing.assert_allclose(vals[:, 2], pick_one_freqs, atol=1e-5)
    np.testing.assert_allclose(vals[:, 3], pick_one_vals, atol=1e-5)
    np.testing.assert_allclose(vals[:, 4], half_shift_freqs, atol=1e-5)
    np.testing.assert_allclose(vals[:, 5], half_shift_vals, atol=1e-5)
