program test_gridsysf
    use, intrinsic :: iso_c_binding
    use gridsysf, only: &
        gridsys_get_bz_grid_addresses, &
        gridsys_get_triplets_at_q
    implicit none

    integer(c_long) :: wurtzite_rec_rotations_without_time_reversal(3, 3, 12)

    wurtzite_rec_rotations_without_time_reversal(:, :, :) = &
        reshape([1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, -1, 0, 0, 0, 0, 1, &
                 0, 1, 0, -1, -1, 0, 0, 0, 1, -1, 0, 0, 0, -1, 0, 0, 0, 1, &
                 -1, -1, 0, 1, 0, 0, 0, 0, 1, 0, -1, 0, 1, 1, 0, 0, 0, 1, &
                 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, -1, 0, 0, 0, 1, &
                 1, 0, 0, -1, -1, 0, 0, 0, 1, 0, -1, 0, -1, 0, 0, 0, 0, 1, &
                 -1, -1, 0, 0, 1, 0, 0, 0, 1, -1, 0, 0, 1, 1, 0, 0, 0, 1], [3, 3, 12])

    write (*, '("[test_gridsys_get_bz_grid_addresses]")')
    call test_gridsys_get_bz_grid_addresses()
    write (*, '("[test_gridsys_get_triplets_at_q]")')
    call test_gridsys_get_triplets_at_q()

contains
    subroutine test_gridsys_get_bz_grid_addresses() bind(C)
        integer(c_long) :: bz_size
        integer(c_long) :: PS(3), D_diag(3), Q(3, 3), bz_grid_addresses(3, 144)
        integer(c_long) :: bz_map(76), bzg2grg(144)
        real(c_double) :: rec_lattice(3, 3)
        integer :: i, j

        integer(c_long) :: ref_bz_grid_addresses(3, 93)
        integer(c_long) :: ref_bz_map(76)
        integer(c_long) :: ref_bzg2grg(93)

        ref_bz_grid_addresses(:, :) = &
            reshape([0, 0, 0, 1, 0, 0, 2, 0, 0, -2, 0, 0, -1, 0, 0, &
                     0, 1, 0, 1, 1, 0, 2, 1, 0, -3, 1, 0, -2, 1, 0, &
                     -1, 1, 0, 0, 2, 0, 1, 2, 0, 1, -3, 0, 2, -3, 0, &
                     -3, 2, 0, -2, 2, 0, -1, 2, 0, 0, -2, 0, 1, -2, 0, &
                     2, -2, 0, -2, 3, 0, 3, -2, 0, -1, -2, 0, -1, 3, 0, &
                     0, -1, 0, 1, -1, 0, 2, -1, 0, -2, -1, 0, 3, -1, 0, &
                     -1, -1, 0, 0, 0, 1, 1, 0, 1, 2, 0, 1, -2, 0, 1, &
                     -1, 0, 1, 0, 1, 1, 1, 1, 1, 2, 1, 1, -3, 1, 1, &
                     -2, 1, 1, -1, 1, 1, 0, 2, 1, 1, 2, 1, 1, -3, 1, &
                     2, -3, 1, -3, 2, 1, -2, 2, 1, -1, 2, 1, 0, -2, 1, &
                     1, -2, 1, 2, -2, 1, -2, 3, 1, 3, -2, 1, -1, -2, 1, &
                     -1, 3, 1, 0, -1, 1, 1, -1, 1, 2, -1, 1, -2, -1, 1, &
                     3, -1, 1, -1, -1, 1, 0, 0, -1, 1, 0, -1, 2, 0, -1, &
                     -2, 0, -1, -1, 0, -1, 0, 1, -1, 1, 1, -1, 2, 1, -1, &
                     -3, 1, -1, -2, 1, -1, -1, 1, -1, 0, 2, -1, 1, 2, -1, &
                     1, -3, -1, 2, -3, -1, -3, 2, -1, -2, 2, -1, -1, 2, -1, &
                     0, -2, -1, 1, -2, -1, 2, -2, -1, -2, 3, -1, 3, -2, -1, &
                     -1, -2, -1, -1, 3, -1, 0, -1, -1, 1, -1, -1, 2, -1, -1, &
                     -2, -1, -1, 3, -1, -1, -1, -1, -1], [3, 93])
        ref_bz_map(:) = [0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 14, 16, &
                         17, 18, 19, 20, 21, 23, 25, 26, 27, 28, 30, 31, 32, 33, &
                         34, 35, 36, 37, 38, 40, 41, 42, 43, 45, 47, 48, 49, 50, &
                         51, 52, 54, 56, 57, 58, 59, 61, 62, 63, 64, 65, 66, 67, &
                         68, 69, 71, 72, 73, 74, 76, 78, 79, 80, 81, 82, 83, 85, &
                         87, 88, 89, 90, 92, 93]
        ref_bzg2grg(:) = [ &
                         0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 11, 12, 12, &
                         13, 14, 15, 16, 17, 18, 18, 19, 19, 20, 21, 22, 23, 23, 24, 25, &
                         26, 27, 28, 29, 30, 31, 32, 32, 33, 34, 35, 36, 36, 37, 37, 38, &
                         39, 40, 41, 42, 43, 43, 44, 44, 45, 46, 47, 48, 48, 49, 50, 51, &
                         52, 53, 54, 55, 56, 57, 57, 58, 59, 60, 61, 61, 62, 62, 63, 64, &
                         65, 66, 67, 68, 68, 69, 69, 70, 71, 72, 73, 73, 74]

        PS(:) = [0, 0, 0]
        D_diag(:) = [5, 5, 3]
        Q(:, :) = reshape([1, 0, 0, 0, 1, 0, 0, 0, 1], [3, 3])
        rec_lattice(:, :) = reshape([0.3214400514304082, 0.0, 0.0, &
                                     0.1855835002216734, 0.3711670004433468, 0.0, &
                                     0.0, 0.0, 0.20088388911209323], [3, 3])

        bz_size = gridsys_get_bz_grid_addresses(bz_grid_addresses, bz_map, bzg2grg, &
                                                D_diag, Q, PS, rec_lattice, int(2, c_long))

        write (*, '("check bz_grid_addresses")', advance='no')
        call assert_2D_array_c_long(bz_grid_addresses, ref_bz_grid_addresses, &
                                    shape(ref_bz_grid_addresses))
        write (*, '("  OK")')

        write (*, '("check bz_map")', advance='no')
        call assert_1D_array_c_long(bz_map, ref_bz_map, 76)
        write (*, '("  OK")')

        write (*, '("check bzg2grg")', advance='no')
        call assert_1D_array_c_long(bzg2grg, ref_bzg2grg, 93)
        write (*, '("  OK")')
    end subroutine test_gridsys_get_bz_grid_addresses

    subroutine test_gridsys_get_triplets_at_q() bind(C)
        integer(c_long) :: D_diag(3)
        integer(c_long) :: map_triplets(36), map_q(36)
        integer(c_long) :: rec_rotations(3, 3, 12)
        integer(c_long) :: grid_point, is_time_reversal, num_rot, swappable
        integer :: i, j, k, n_ir_triplets

        integer :: ref_num_triplets(4)
        integer(c_long) :: ref_map_triplets(36, 4), ref_map_q(36, 4)

        grid_point = 1
        D_diag(:) = [3, 3, 4]
        num_rot = 12
        ref_num_triplets(:) = [12, 18, 14, 24]
        ref_map_triplets(:, :) = &
            reshape([ &
                    0, 1, 0, 3, 3, 5, 5, 3, 3, 9, 10, 9, 12, 12, 14, 14, 12, 12, &
                    18, 19, 18, 21, 21, 23, 23, 21, 21, 9, 10, 9, 12, 12, 14, 14, 12, 12, &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    18, 19, 20, 21, 22, 23, 23, 21, 22, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    0, 1, 0, 3, 3, 5, 5, 3, 3, 9, 10, 11, &
                    12, 13, 14, 14, 12, 13, 18, 19, 18, 21, 21, 23, &
                    23, 21, 21, 11, 10, 9, 13, 12, 14, 14, 13, 12, &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, &
                    12, 13, 14, 14, 12, 13, 18, 19, 20, 21, 22, 23, &
                    23, 21, 22, 27, 28, 29, 30, 31, 32, 32, 30, 31 &
                    ], [36, 4])
        ref_map_q(:, :) = &
            reshape([ &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    18, 19, 20, 21, 22, 23, 23, 21, 22, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    18, 19, 20, 21, 22, 23, 23, 21, 22, 9, 10, 11, 12, 13, 14, 14, 12, 13, &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, &
                    12, 13, 14, 14, 12, 13, 18, 19, 20, 21, 22, 23, &
                    23, 21, 22, 27, 28, 29, 30, 31, 32, 32, 30, 31, &
                    0, 1, 2, 3, 4, 5, 5, 3, 4, 9, 10, 11, &
                    12, 13, 14, 14, 12, 13, 18, 19, 20, 21, 22, 23, &
                    23, 21, 22, 27, 28, 29, 30, 31, 32, 32, 30, 31], &
                    [36, 4])

        k = 1
        do i = 0, 1
            do j = 0, 1
                is_time_reversal = 1 - i
                swappable = 1 - j
                n_ir_triplets = gridsys_get_triplets_at_q( &
                                map_triplets, map_q, grid_point, &
                                D_diag, is_time_reversal, num_rot, &
                                wurtzite_rec_rotations_without_time_reversal, &
                                swappable)

                call assert_int(n_ir_triplets, ref_num_triplets(k))
                write (*, '("check map_triplets")', advance='no')
                call assert_1D_array_c_long(map_triplets, ref_map_triplets(:, k), 36)
                write (*, '("  OK")')
                write (*, '("check map_q")', advance='no')
                call assert_1D_array_c_long(map_q, ref_map_q(:, k), 36)
                write (*, '("  OK")')
                k = k + 1
            end do
        end do
    end subroutine test_gridsys_get_triplets_at_q

    subroutine assert_int(val, ref_val)
        integer, intent(in) :: val, ref_val
        if (val /= ref_val) then
            print '()'
            print '(i0, "/=", i0)', val, ref_val
            error stop
        end if
    end subroutine assert_int

    subroutine assert_1D_array_c_long(vals, ref_vals, size_of_array)
        integer(c_long), intent(in) :: vals(:)
        integer(c_long), intent(in) :: ref_vals(:)
        integer, intent(in) :: size_of_array
        integer :: i

        do i = 1, size_of_array
            if (vals(i) /= ref_vals(i)) then
                print '()'
                print '(i0, ":", i0, " ", i0)', i, vals(i), ref_vals(i)
                error stop
            end if
        end do
    end subroutine assert_1D_array_c_long

    subroutine assert_2D_array_c_long(vals, ref_vals, shape_of_array)
        integer(c_long), intent(in) :: vals(:, :)
        integer(c_long), intent(in) :: ref_vals(:, :)
        integer, intent(in) :: shape_of_array(:)
        integer :: i, j

        do i = 1, shape_of_array(1)
            do j = 1, shape_of_array(2)
                if (vals(j, i) /= ref_vals(j, i)) then
                    print '()'
                    print '("(", i0, ",", i0, "):", i0, " ", i0)', i, j, vals(j, i), ref_vals(j, i)
                    error stop
                end if
            end do
        end do
    end subroutine assert_2D_array_c_long

end program test_gridsysf
