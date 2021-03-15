/* Copyright (C) 2015 Atsushi Togo */
/* All rights reserved. */

/* These codes were originally parts of spglib, but only develped */
/* and used for phono3py. Therefore these were moved from spglib to */
/* phono3py. This file is part of phonopy. */

/* Redistribution and use in source and binary forms, with or without */
/* modification, are permitted provided that the following conditions */
/* are met: */

/* * Redistributions of source code must retain the above copyright */
/*   notice, this list of conditions and the following disclaimer. */

/* * Redistributions in binary form must reproduce the above copyright */
/*   notice, this list of conditions and the following disclaimer in */
/*   the documentation and/or other materials provided with the */
/*   distribution. */

/* * Neither the name of the phonopy project nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission. */

/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE */
/* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, */
/* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, */
/* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT */
/* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN */
/* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE */
/* POSSIBILITY OF SUCH DAMAGE. */

#include <stddef.h>
#include <stdlib.h>
#include "bzgrid.h"
#include "grgrid.h"
#include "lagrid.h"
#include "triplet.h"
#include "triplet_grid.h"

#define BZG_NUM_BZ_SEARCH_SPACE 125
static long bz_search_space[BZG_NUM_BZ_SEARCH_SPACE][3] = {
  { 0,  0,  0},
  { 0,  0,  1},
  { 0,  0,  2},
  { 0,  0, -2},
  { 0,  0, -1},
  { 0,  1,  0},
  { 0,  1,  1},
  { 0,  1,  2},
  { 0,  1, -2},
  { 0,  1, -1},
  { 0,  2,  0},
  { 0,  2,  1},
  { 0,  2,  2},
  { 0,  2, -2},
  { 0,  2, -1},
  { 0, -2,  0},
  { 0, -2,  1},
  { 0, -2,  2},
  { 0, -2, -2},
  { 0, -2, -1},
  { 0, -1,  0},
  { 0, -1,  1},
  { 0, -1,  2},
  { 0, -1, -2},
  { 0, -1, -1},
  { 1,  0,  0},
  { 1,  0,  1},
  { 1,  0,  2},
  { 1,  0, -2},
  { 1,  0, -1},
  { 1,  1,  0},
  { 1,  1,  1},
  { 1,  1,  2},
  { 1,  1, -2},
  { 1,  1, -1},
  { 1,  2,  0},
  { 1,  2,  1},
  { 1,  2,  2},
  { 1,  2, -2},
  { 1,  2, -1},
  { 1, -2,  0},
  { 1, -2,  1},
  { 1, -2,  2},
  { 1, -2, -2},
  { 1, -2, -1},
  { 1, -1,  0},
  { 1, -1,  1},
  { 1, -1,  2},
  { 1, -1, -2},
  { 1, -1, -1},
  { 2,  0,  0},
  { 2,  0,  1},
  { 2,  0,  2},
  { 2,  0, -2},
  { 2,  0, -1},
  { 2,  1,  0},
  { 2,  1,  1},
  { 2,  1,  2},
  { 2,  1, -2},
  { 2,  1, -1},
  { 2,  2,  0},
  { 2,  2,  1},
  { 2,  2,  2},
  { 2,  2, -2},
  { 2,  2, -1},
  { 2, -2,  0},
  { 2, -2,  1},
  { 2, -2,  2},
  { 2, -2, -2},
  { 2, -2, -1},
  { 2, -1,  0},
  { 2, -1,  1},
  { 2, -1,  2},
  { 2, -1, -2},
  { 2, -1, -1},
  {-2,  0,  0},
  {-2,  0,  1},
  {-2,  0,  2},
  {-2,  0, -2},
  {-2,  0, -1},
  {-2,  1,  0},
  {-2,  1,  1},
  {-2,  1,  2},
  {-2,  1, -2},
  {-2,  1, -1},
  {-2,  2,  0},
  {-2,  2,  1},
  {-2,  2,  2},
  {-2,  2, -2},
  {-2,  2, -1},
  {-2, -2,  0},
  {-2, -2,  1},
  {-2, -2,  2},
  {-2, -2, -2},
  {-2, -2, -1},
  {-2, -1,  0},
  {-2, -1,  1},
  {-2, -1,  2},
  {-2, -1, -2},
  {-2, -1, -1},
  {-1,  0,  0},
  {-1,  0,  1},
  {-1,  0,  2},
  {-1,  0, -2},
  {-1,  0, -1},
  {-1,  1,  0},
  {-1,  1,  1},
  {-1,  1,  2},
  {-1,  1, -2},
  {-1,  1, -1},
  {-1,  2,  0},
  {-1,  2,  1},
  {-1,  2,  2},
  {-1,  2, -2},
  {-1,  2, -1},
  {-1, -2,  0},
  {-1, -2,  1},
  {-1, -2,  2},
  {-1, -2, -2},
  {-1, -2, -1},
  {-1, -1,  0},
  {-1, -1,  1},
  {-1, -1,  2},
  {-1, -1, -2},
  {-1, -1, -1}
};

static long get_ir_triplets_at_q(long *map_triplets,
                                 long *map_q,
                                 const long grid_point,
                                 const long D_diag[3],
                                 const RotMats * rot_reciprocal,
                                 const long swappable);
static long get_BZ_triplets_at_q(long (*triplets)[3],
                                 const long grid_point,
                                 const ConstBZGrid *bzgrid,
                                 const long *map_triplets);
static void get_BZ_triplets_at_q_type1(long (*triplets)[3],
                                       const long grid_point,
                                       const ConstBZGrid *bzgrid,
                                       const long *ir_grid_points,
                                       const long num_ir);
static void get_BZ_triplets_at_q_type2(long (*triplets)[3],
                                       const long grid_point,
                                       const ConstBZGrid *bzgrid,
                                       const long *ir_grid_points,
                                       const long num_ir);
static long get_third_q_of_triplets_at_q_type1(long bz_address[3][3],
                                               const long q_index,
                                               const ConstBZGrid *bzgrid,
                                               const long bzmesh[3],
                                               const long QD[3]);
static void get_LQD_inv(double LQD_inv[3][3], const ConstBZGrid *bzgrid);
static RotMats *get_point_group_reciprocal_with_q(const RotMats * rot_reciprocal,
                                                  const long D_diag[3],
                                                  const long grid_point);
static void modulo_l3(long v[3], const long m[3]);

long tpk_get_ir_triplets_at_q(long *map_triplets,
                              long *map_q,
                              const long grid_point,
                              const long D_diag[3],
                              const long is_time_reversal,
                              const long (*rotations)[3][3],
                              const long num_rot,
                              const long swappable)
{
  long i, num_ir;
  RotMats *rot_real, *rot_reciprocal;

  rot_real = bzg_alloc_RotMats(num_rot);
  for (i = 0; i < num_rot; i++) {
    lagmat_copy_matrix_l3(rot_real->mat[i], rotations[i]);
  }
  rot_reciprocal = bzg_get_point_group_reciprocal(rot_real, is_time_reversal);
  bzg_free_RotMats(rot_real);

  num_ir = get_ir_triplets_at_q(map_triplets,
                                map_q,
                                grid_point,
                                D_diag,
                                rot_reciprocal,
                                swappable);
  bzg_free_RotMats(rot_reciprocal);

  return num_ir;
}

long tpk_get_BZ_triplets_at_q(long (*triplets)[3],
                              const long grid_point,
                              const ConstBZGrid *bzgrid,
                              const long *map_triplets)
{
  return get_BZ_triplets_at_q(triplets,
                              grid_point,
                              bzgrid,
                              map_triplets);
}

static long get_ir_triplets_at_q(long *map_triplets,
                                 long *map_q,
                                 const long grid_point,
                                 const long D_diag[3],
                                 const RotMats * rot_reciprocal,
                                 const long swappable)
{
  long i, j, num_grid, q_2, num_ir_q, num_ir_triplets, ir_gp;
  long PS[3];
  long adrs0[3], adrs1[3], adrs2[3];
  long *ir_grid_points, *third_q;
  RotMats *rot_reciprocal_q;

  ir_grid_points = NULL;
  third_q = NULL;
  rot_reciprocal_q = NULL;
  num_ir_triplets = 0;

  num_grid = D_diag[0] * D_diag[1] * D_diag[2];

  for (i = 0; i < 3; i++) {
    PS[i] = 0;
  }

  /* Search irreducible q-points (map_q) with a stabilizer. */
  rot_reciprocal_q = get_point_group_reciprocal_with_q(rot_reciprocal,
                                                       D_diag,
                                                       grid_point);
  num_ir_q = bzg_get_ir_grid_map(map_q,
                                 D_diag,
                                 PS,
                                 rot_reciprocal_q);
  bzg_free_RotMats(rot_reciprocal_q);
  rot_reciprocal_q = NULL;

  if ((third_q = (long*) malloc(sizeof(long) * num_ir_q)) == NULL) {
    warning_print("Memory could not be allocated.");
    goto ret;
  }

  if ((ir_grid_points = (long*) malloc(sizeof(long) * num_ir_q)) == NULL) {
    warning_print("Memory could not be allocated.");
    goto ret;
  }

  num_ir_q = 0;
  for (i = 0; i < num_grid; i++) {
    if (map_q[i] == i) {
      ir_grid_points[num_ir_q] = i;
      num_ir_q++;
    }
  }

  for (i = 0; i < num_grid; i++) {
    map_triplets[i] = num_grid;  /* When not found, map_triplets == num_grid */
  }

  grg_get_grid_address_from_index(adrs0, grid_point, D_diag);

#pragma omp parallel for private(j, adrs1, adrs2)
  for (i = 0; i < num_ir_q; i++) {
    grg_get_grid_address_from_index(adrs1, ir_grid_points[i], D_diag);
    for (j = 0; j < 3; j++) { /* q'' */
      adrs2[j] = - adrs0[j] - adrs1[j];
    }
    third_q[i] = grg_get_grid_index(adrs2, D_diag);
  }

  if (swappable) { /* search q1 <-> q2 */
    for (i = 0; i < num_ir_q; i++) {
      ir_gp = ir_grid_points[i];
      q_2 = third_q[i];
      if (map_triplets[map_q[q_2]] < num_grid) {
        map_triplets[ir_gp] = map_triplets[map_q[q_2]];
      } else {
        map_triplets[ir_gp] = ir_gp;
        num_ir_triplets++;
      }
    }
  } else {
    for (i = 0; i < num_ir_q; i++) {
      ir_gp = ir_grid_points[i];
      map_triplets[ir_gp] = ir_gp;
      num_ir_triplets++;
    }
  }

#pragma omp parallel for
  for (i = 0; i < num_grid; i++) {
    map_triplets[i] = map_triplets[map_q[i]];
  }

ret:
  if (third_q) {
    free(third_q);
    third_q = NULL;
  }
  if (ir_grid_points) {
    free(ir_grid_points);
    ir_grid_points = NULL;
  }
  return num_ir_triplets;
}

static long get_BZ_triplets_at_q(long (*triplets)[3],
                                 const long grid_point,
                                 const ConstBZGrid *bzgrid,
                                 const long *map_triplets)
{
  long i, num_ir;
  long *ir_grid_points;

  ir_grid_points = NULL;
  num_ir = 0;

  if ((ir_grid_points = (long*) malloc(sizeof(long) * bzgrid->size))
      == NULL) {
    warning_print("Memory could not be allocated.");
    goto ret;
  }

  for (i = 0; i < bzgrid->size; i++) {
    if (map_triplets[i] == i) {
      ir_grid_points[num_ir] = i;
      num_ir++;
    }
  }

  if (bzgrid->type == 1) {
    get_BZ_triplets_at_q_type1(triplets,
                               grid_point,
                               bzgrid,
                               ir_grid_points,
                               num_ir);
  } else {
    get_BZ_triplets_at_q_type2(triplets,
                               grid_point,
                               bzgrid,
                               ir_grid_points,
                               num_ir);
  }

  free(ir_grid_points);
  ir_grid_points = NULL;

ret:
  return num_ir;
}

static void get_BZ_triplets_at_q_type1(long (*triplets)[3],
                                       const long grid_point,
                                       const ConstBZGrid *bzgrid,
                                       const long *ir_grid_points,
                                       const long num_ir)
{
  long i, j;
  long bz_address[3][3], bzmesh[3], QD[3];

  for (i = 0; i < 3; i++) {
    bzmesh[i] = bzgrid->D_diag[i] * 2;
  }
  lagmat_multiply_matrix_vector_l3(QD, bzgrid->Q, bzgrid->D_diag);

#pragma omp parallel for private(j, bz_address)
  for (i = 0; i < num_ir; i++) {
    for (j = 0; j < 3; j++) {
      bz_address[0][j] = bzgrid->addresses[grid_point][j];
      bz_address[1][j] = bzgrid->addresses[ir_grid_points[i]][j];
      bz_address[2][j] = - bz_address[0][j] - bz_address[1][j];
    }
    for (j = 2; j > -1; j--) {
      if (get_third_q_of_triplets_at_q_type1(bz_address,
                                             j,
                                             bzgrid,
                                             bzmesh,
                                             QD) == 0) {
        break;
      }
    }
    for (j = 0; j < 3; j++) {
      triplets[i][j] = bzgrid->gp_map[
        grg_get_grid_index(bz_address[j], bzmesh)];
    }
  }
}

static void get_BZ_triplets_at_q_type2(long (*triplets)[3],
                                       const long grid_point,
                                       const ConstBZGrid *bzgrid,
                                       const long *ir_grid_points,
                                       const long num_ir)
{
  long i, j, gp0, gp2;
  long bzgp[3], G[3];
  long bz_adrs0[3], bz_adrs1[3], bz_adrs2[3];
  const long *gp_map;
  const long (*bz_adrs)[3];
  double d, d2, min_d2, tolerance;
  double LQD_inv[3][3];

  gp_map = bzgrid->gp_map;
  bz_adrs = bzgrid->addresses;
  get_LQD_inv(LQD_inv, bzgrid);
  /* This tolerance is used to be consistent to BZ reduction in bzgrid. */
  tolerance = bzg_get_tolerance_for_BZ_reduction((BZGrid*)bzgrid);

  for (i = 0; i < 3; i++) {
    bz_adrs0[i] = bz_adrs[grid_point][i];
  }
  gp0 = grg_get_grid_index(bz_adrs0, bzgrid->D_diag);

#pragma omp parallel for private(j, gp2, bzgp, G, bz_adrs1, bz_adrs2, d, d2, min_d2)
  for (i = 0; i < num_ir; i++) {
    for (j = 0; j < 3; j++) {
      bz_adrs1[j] = bz_adrs[gp_map[ir_grid_points[i]]][j];
      bz_adrs2[j] = - bz_adrs0[j] - bz_adrs1[j];
    }
    gp2 = grg_get_grid_index(bz_adrs2, bzgrid->D_diag);
    /* Negative value is the signal to initialize min_d2 later. */
    min_d2 = -1;
    for (bzgp[0] = gp_map[gp0]; bzgp[0] < gp_map[gp0 + 1]; bzgp[0]++) {
      for (bzgp[1] = gp_map[ir_grid_points[i]];
           bzgp[1] < gp_map[ir_grid_points[i] + 1]; bzgp[1]++) {
        for (bzgp[2] = gp_map[gp2]; bzgp[2] < gp_map[gp2 + 1]; bzgp[2]++) {
          for (j = 0; j < 3; j++) {
            G[j] = bz_adrs[bzgp[0]][j] + bz_adrs[bzgp[1]][j] + bz_adrs[bzgp[2]][j];
          }
          if (G[0] == 0 && G[1] == 0 && G[2] == 0) {
            for (j = 0; j < 3; j++) {
              triplets[i][j] = bzgp[j];
            }
            goto found;
          }
          d2 = 0;
          for (j = 0; j < 3; j++) {
            d = LQD_inv[j][0] * G[0] + LQD_inv[j][1] * G[1] + LQD_inv[j][2] * G[2];
            d2 += d * d;
          }
          if (d2 < min_d2 + tolerance || min_d2 < 0) {
            min_d2 = d2;
            for (j = 0; j < 3; j++) {
              triplets[i][j] = bzgp[j];
            }
          }
        }
      }
    }
  found:
    ;
  }
}

static long get_third_q_of_triplets_at_q_type1(long bz_address[3][3],
                                               const long q_index,
                                               const ConstBZGrid *bzgrid,
                                               const long bzmesh[3],
                                               const long QD[3])
{
  long i, j, smallest_g, smallest_index, sum_g, delta_g[3];
  long prod_bzmesh;
  long bzgp[BZG_NUM_BZ_SEARCH_SPACE];
  long bz_address_search[3];
  const long *bz_map;

  bz_map = bzgrid->gp_map;
  prod_bzmesh = bzmesh[0] * bzmesh[1] * bzmesh[2];

  modulo_l3(bz_address[q_index], bzgrid->D_diag);
  for (i = 0; i < 3; i++) {
    delta_g[i] = 0;
    for (j = 0; j < 3; j++) {
      delta_g[i] += bz_address[j][i];
    }
    delta_g[i] /= bzgrid->D_diag[i];
  }

  for (i = 0; i < BZG_NUM_BZ_SEARCH_SPACE; i++) {
    for (j = 0; j < 3; j++) {
      bz_address_search[j]
        = bz_address[q_index][j] + bz_search_space[i][j] * bzgrid->D_diag[j];
    }
    bzgp[i] = bz_map[grg_get_grid_index(bz_address_search, bzmesh)];
  }

  for (i = 0; i < BZG_NUM_BZ_SEARCH_SPACE; i++) {
    if (bzgp[i] != prod_bzmesh) {
      goto escape;
    }
  }

escape:

  smallest_g = 4;
  smallest_index = 0;

  for (i = 0; i < BZG_NUM_BZ_SEARCH_SPACE; i++) {
    if (bzgp[i] < prod_bzmesh) { /* q'' is in BZ */
      sum_g = (labs(delta_g[0] + bz_search_space[i][0]) +
               labs(delta_g[1] + bz_search_space[i][1]) +
               labs(delta_g[2] + bz_search_space[i][2]));
      if (sum_g < smallest_g) {
        smallest_index = i;
        smallest_g = sum_g;
      }
    }
  }

  for (i = 0; i < 3; i++) {
    bz_address[q_index][i] += bz_search_space[smallest_index][i] * bzgrid->D_diag[i];
  }

  return smallest_g;
}

static void get_LQD_inv(double LQD_inv[3][3], const ConstBZGrid *bzgrid)
{
  long i, j, k;

  /* LQD^-1 */
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      for (k = 0; k < 3; k++) {
        LQD_inv[i][k]
          = bzgrid->reclat[i][j] * bzgrid->Q[j][k] / bzgrid->D_diag[k];
      }
    }
  }
}

/* Return NULL if failed */
static RotMats *get_point_group_reciprocal_with_q(const RotMats * rot_reciprocal,
                                                  const long D_diag[3],
                                                  const long grid_point)
{
  long i, num_rot, gp_rot;
  long *ir_rot;
  long adrs[3], adrs_rot[3];
  RotMats * rot_reciprocal_q;

  ir_rot = NULL;
  rot_reciprocal_q = NULL;
  num_rot = 0;

  grg_get_grid_address_from_index(adrs, grid_point, D_diag);

  if ((ir_rot = (long*)malloc(sizeof(long) * rot_reciprocal->size)) == NULL) {
    warning_print("Memory of ir_rot could not be allocated.");
    return NULL;
  }

  for (i = 0; i < rot_reciprocal->size; i++) {
    ir_rot[i] = -1;
  }
  for (i = 0; i < rot_reciprocal->size; i++) {
    lagmat_multiply_matrix_vector_l3(adrs_rot, rot_reciprocal->mat[i], adrs);
    gp_rot = grg_get_grid_index(adrs_rot, D_diag);

    if (gp_rot == grid_point) {
      ir_rot[num_rot] = i;
      num_rot++;
    }
  }

  if ((rot_reciprocal_q = bzg_alloc_RotMats(num_rot)) != NULL) {
    for (i = 0; i < num_rot; i++) {
      lagmat_copy_matrix_l3(rot_reciprocal_q->mat[i],
                            rot_reciprocal->mat[ir_rot[i]]);
    }
  }

  free(ir_rot);
  ir_rot = NULL;

  return rot_reciprocal_q;
}

static void modulo_l3(long v[3], const long m[3])
{
  long i;

  for (i = 0; i < 3; i++) {
    v[i] = v[i] % m[i];

    if (v[i] < 0) {
      v[i] += m[i];
    }
  }
}
