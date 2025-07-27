#
# This is the actual makefile that should be run by GNU Make (gmake)
#

#
# Makefile for VaSTv1
#
# Chenge it only if 'make' (or 'gmake') fails and you think you know why...
#


# If PGPLOT or related components fail to compile complaining about missing X11 libraries,
# please make sure you have all the -devel packages related to X11 installed.
# If the problem persist - try to specify X11 library manually below.

# X11 library path
# X11_LIB_PATH = /usr/X11R6/lib
# try to set the correct path to X11 library, but it is most likely the program
# will compile fine even if this path is set incorrectly
# If RECOMPILE_VAST_ONLY is set to 'yes' only VaST will be re-compiled (faster option for developement) 
#RECOMPILE_VAST_ONLY = yes

# You probably don't want to change anything below this line
##############################################################################

SRC_PATH = src/
LIB_IDENT_PATH = $(SRC_PATH)
LIB_DIR = lib/
SOURCE_IDENT_PATH = $(SRC_PATH)

CC := $(shell lib/find_gcc_compiler.sh)

USE_OMP_OPTIONS := $(shell lib/set_openmp.sh)
LTO_OPTIONS := $(shell lib/set_lto.sh)
GOOD_MARCH_OPTIONS := $(shell lib/set_good_march.sh)
USE_SINCOS_OPITION := $(shell lib/check_sincos.sh)
USE_BUILTIN_FUNCTIONS := $(shell lib/check_builtin_functions.sh)

# Check which zlib library to use
ZLIB_LIB := $(shell lib/check_zlib.sh 2>/dev/null || echo "")

CFITSIO_LIB = lib/libcfitsio.a $(ZLIB_LIB)
GSL_LIB = lib/lib/libgsl.a lib/lib/libgslcblas.a
GSL_INCLUDE = lib/include

## production
OPTFLAGS = -w -O2 -fomit-frame-pointer $(GOOD_MARCH_OPTIONS) $(LTO_OPTIONS) $(USE_OMP_OPTIONS) $(USE_BUILTIN_FUNCTIONS)
# jemalloc test - nope, it is not faster than glibc malloc
#OPTFLAGS = -ljemalloc -w -O2 -fomit-frame-pointer $(GOOD_MARCH_OPTIONS) $(LTO_OPTIONS) $(USE_OMP_OPTIONS) $(USE_BUILTIN_FUNCTIONS)
## production with warnings
#OPTFLAGS = -Wall -Wno-comment -O2 -fomit-frame-pointer $(GOOD_MARCH_OPTIONS) $(LTO_OPTIONS) $(USE_OMP_OPTIONS) $(USE_BUILTIN_FUNCTIONS)
# more conservative production flags
#OPTFLAGS = -O2 -w $(GOOD_MARCH_OPTIONS)
## debug with Valgrind
#OPTFLAGS := -g -Wall -Wno-comment -Wno-error -Warray-bounds -Wextra -fno-omit-frame-pointer -O0 $(USE_BUILTIN_FUNCTIONS) #$(USE_OMP_OPTIONS) # for debugging with valgrind
#OPTFLAGS := -g -Wall -Wno-comment -Wno-error -Warray-bounds -Wextra -fno-omit-frame-pointer -fstack-protector-all -O0 $(USE_OMP_OPTIONS) $(USE_BUILTIN_FUNCTIONS) #$(USE_OMP_OPTIONS) # for debugging with valgrind
## Address Sanitizer (not compatible with Valgrind)
#OPTFLAGS := -g -Wall -Wno-comment  -fsanitize=address,undefined -fsanitize-address-use-after-scope -O1 $(USE_BUILTIN_FUNCTIONS) 
#OPTFLAGS := -g -Wall -Wno-comment  -fsanitize=address,leak,undefined -fsanitize-address-use-after-scope -O1 $(USE_BUILTIN_FUNCTIONS) 
## flags to measure test coverage with GCC/gcov
#OPTFLAGS = -fprofile-arcs -ftest-coverage
## Static Analysis
#OPTFLAGS := -g -Wall -Wpedantic -Wno-comment  -fanalyzer

##############################################################################
########################### Make targets are below ###########################
##############################################################################

# this invocation of make will be run serially, even if the '-j' option is given.
.NOTPARALLEL:



all: print_check_start_message check print_compile_start_message clean check_no_for_loop_initial_declaration shell_commands_record_compiler_version zlib cfitsio gsl sextractor wcstools set_vast_limits vast.o vast statistics etc pgplot_components old shell_commands period_filter ccd astrometry astcheck cdsclient test  clean_objects print_compile_success_message

ifneq ($(RECOMPILE_VAST_ONLY),yes)
check:
	lib/check_external_programs.sh $(CC)
else
check:
	# do nothing
endif

q: vast statistics etc pgplot_components old period_filter ccd

main: vast.o vast statistics stetson_test lib/create_data

statistics: m_sigma_bin index_vs_mag select_sysrem_input_star_list drop lib/select_only_n_random_points_from_set_of_lightcurves lib/new_lightcurve_sigma_filter lib/select_aperture_with_smallest_scatter_for_each_object lib/create_data rescale_photometric_errors util/colstat util/imstat_vast

etc: stat_outfile util/calibrate_magnitude_scale lib/deg2hms lib/coord_v_dva_slova lib/hms2deg lib/fix_photo_log util/sysrem util/sysrem2 lib/lightcurve_simulator lib/noise_lightcurve_simulator util/local_zeropoint_correction lib/checkstar lib/remove_bad_images lib/sort_all_lightcurve_files_in_jd lib/put_two_sources_in_one_field lib/fit_parabola_wpolyfit lib/remove_lightcurves_with_small_number_of_points lib/transient_list util/hjd util/convert/CoRoT_FITS2ASCII util/convert/SWASP_FITS2ASCII util/cute_lc util/observations_per_star lib/kwee-van-woerden lib/find_star_in_wcs_catalog util/UTC2TT lib/find_flares lib/catalogs/read_tycho2 lib/catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search lib/catalogs/check_catalogs_offline util/get_image_date lib/fast_clean_data stetson_test util/split_multiextension_fits lib/guess_saturation_limit_main lib/shutterless_bad_regions_hack lib/MagSize_filter_standalone util/phase_lc lib/on_the_fly_symlink_or_convert util/bin_lightcurve_in_time lib/ConstellationBoundaries

old: formater_out_wfk 

astrometry: lib/astrometry/get_image_dimentions lib/astrometry/insert_wcs_header lib/astrometry/strip_wcs_keywords lib/make_outxyls_for_astrometric_calibration util/solve_plate_with_UCAC5 lib/autodetect_aperture_main lib/try_to_guess_image_fov cfitsio gsl

pgplot_components: variability_indexes.o photocurve.o gettime.o kourovka_sbg_date.o autodetect_aperture.o guess_saturation_limit.o get_number_of_cpu_cores.o exclude_region.o replace_file_with_symlink_if_filename_contains_white_spaces.o wpolyfit.o get_path_to_vast.o is_point_close_or_off_the_frame_edge.o cfitsio gsl
	lib/test_libpng.sh
	echo $(OPTFLAGS) > optflags_for_scripts.tmp
	$(CC) $(OPTFLAGS) -c src/setenv_local_pgplot.c
	lib/compile_pgplot_related_components.sh $(CC) $(CFITSIO_LIB) $(X11_LIB_PATH)
	rm -f optflags_for_scripts.tmp


set_vast_limits: lib/set_MAX_MEASUREMENTS_IN_RAM_in_vast_limits.sh
	lib/set_MAX_MEASUREMENTS_IN_RAM_in_vast_limits.sh

ifneq ($(RECOMPILE_VAST_ONLY),yes)
zlib: shell_commands_record_compiler_version
	lib/compile_zlib.sh
cfitsio: zlib shell_commands_record_compiler_version
	lib/compile_cfitsio.sh
gsl: shell_commands_record_compiler_version
	lib/compile_gsl.sh
sextractor: shell_commands_record_compiler_version
	lib/compile_sextractor.sh
wcstools: shell_commands_record_compiler_version
	lib/compile_wcstools.sh
test: vast
	@echo " "
	@echo -e "\033[01;34mTesting VaST installation...\033[00m"
	@echo " "
	./vast
	lib/look_for_sextractor.sh
	@echo " "
	@echo -e "\033[01;34mVaST installation test passed\033[00m"
	@echo " "
else
zlib:
	# do nothing
cfitsio:
	# do nothing
gsl:
	# do nothing
sextractor:
	# do nothing
wcstools:
	# do nothing
test: vast
	# do nothing
endif


astcheck:
	lib/compile_astcheck.sh
cdsclient:
	lib/compile_cdsclient.sh

period_filter: lib/period_search/periodFilter/periodS2 lib/periodFilter/periodFilter lib/BLS/bls lib/lk_compute_periodogram lib/deeming_compute_periodogram

nopgplot: print_check_start_message check print_compile_start_message clean zlib cfitsio gsl sextractor vast.o vast statistics etc old shell_commands period_filter ccd astrometry astcheck clean_objects print_compile_success_message 


stetson_test: stetson_test.o variability_indexes.o lib/create_data
	$(CC) $(OPTFLAGS) -o lib/test/stetson_test stetson_test.o variability_indexes.o $(GSL_LIB) -lm

stetson_test.o: $(SRC_PATH)test/stetson_test.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)test/stetson_test.c -I$(GSL_INCLUDE)



vast: vast.o vast_image_quality.o vast_utils.o gettime.o kourovka_sbg_date.o vast_report_memory_error.o libident.o autodetect_aperture.o guess_saturation_limit.o exclude_region.o wpolyfit.o photocurve.o fit_plane_lin.o get_number_of_cpu_cores.o replace_file_with_symlink_if_filename_contains_white_spaces.o variability_indexes.o filter_MagSize.o erfinv.o is_point_close_or_off_the_frame_edge.o get_path_to_vast.o detection_limit.o cfitsio gsl
	$(CC) $(OPTFLAGS) -o vast vast.o vast_image_quality.o vast_utils.o gettime.o kourovka_sbg_date.o autodetect_aperture.o guess_saturation_limit.o exclude_region.o wpolyfit.o photocurve.o fit_plane_lin.o get_number_of_cpu_cores.o vast_report_memory_error.o libident.o replace_file_with_symlink_if_filename_contains_white_spaces.o variability_indexes.o filter_MagSize.o erfinv.o is_point_close_or_off_the_frame_edge.o get_path_to_vast.o detection_limit.o $(CFITSIO_LIB) $(GSL_LIB) -Wl,-rpath,$(LIB_IDENT_PATH) -lm

vast.o: $(SRC_PATH)vast.c $(SOURCE_IDENT_PATH)ident.h $(SRC_PATH)vast_limits.h $(SRC_PATH)vast_report_memory_error.h $(SRC_PATH)detailed_error_messages.h $(SRC_PATH)photocurve.h $(SRC_PATH)get_number_of_cpu_cores.h $(SRC_PATH)fit_plane_lin.h $(SRC_PATH)fitsfile_read_check.h $(SRC_PATH)wpolyfit.h $(SRC_PATH)replace_file_with_symlink_if_filename_contains_white_spaces.h $(SRC_PATH)lightcurve_io.h
	$(CC) $(OPTFLAGS) -c -o vast.o $(SRC_PATH)vast.c -I$(GSL_INCLUDE) -Wall
vast_image_quality.o: $(SRC_PATH)vast_image_quality.c
	$(CC) $(OPTFLAGS) -c -o vast_image_quality.o $(SRC_PATH)vast_image_quality.c -I$(GSL_INCLUDE)
vast_utils.o: $(SRC_PATH)vast_utils.c
	$(CC) $(OPTFLAGS) -c -o vast_utils.o $(SRC_PATH)vast_utils.c
gettime.o: $(SRC_PATH)gettime.c
	$(CC) $(OPTFLAGS) -c -o gettime.o $(SRC_PATH)gettime.c
kourovka_sbg_date.o: $(SRC_PATH)kourovka_sbg_date.c
	$(CC) $(OPTFLAGS) -c -o kourovka_sbg_date.o $(SRC_PATH)kourovka_sbg_date.c
autodetect_aperture.o: $(SRC_PATH)autodetect_aperture.c
	$(CC) $(OPTFLAGS) -c  -o autodetect_aperture.o $(SRC_PATH)autodetect_aperture.c -I$(GSL_INCLUDE) -I$(SOURCE_IDENT_PATH)
exclude_region.o: $(SRC_PATH)exclude_region.c
	$(CC) $(OPTFLAGS) -c  -o exclude_region.o $(SRC_PATH)exclude_region.c
wpolyfit.o: $(SRC_PATH)wpolyfit.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)wpolyfit.c -I$(GSL_INCLUDE) 
get_number_of_measured_images_from_vast_summary_log.o: src/get_number_of_measured_images_from_vast_summary_log.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)get_number_of_measured_images_from_vast_summary_log.c
get_number_of_cpu_cores.o: $(SRC_PATH)get_number_of_cpu_cores.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)get_number_of_cpu_cores.c
vast_report_memory_error.o: $(SRC_PATH)vast_report_memory_error.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)vast_report_memory_error.c
replace_file_with_symlink_if_filename_contains_white_spaces.o: $(SRC_PATH)replace_file_with_symlink_if_filename_contains_white_spaces.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)replace_file_with_symlink_if_filename_contains_white_spaces.c
get_path_to_vast.o: $(SRC_PATH)get_path_to_vast.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)get_path_to_vast.c
guess_saturation_limit.o: $(SRC_PATH)guess_saturation_limit.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)guess_saturation_limit.c -I$(GSL_INCLUDE)
detection_limit.o: $(SRC_PATH)detection_limit.c
	 $(CC) $(OPTFLAGS) -c $(SRC_PATH)detection_limit.c -I$(GSL_INCLUDE)

filter_MagSize.o: $(SRC_PATH)filter_MagSize.c
	$(CC) $(OPTFLAGS) -c -o filter_MagSize.o $(SRC_PATH)filter_MagSize.c -I$(GSL_INCLUDE)

erfinv.o: $(SRC_PATH)erfinv.c
	$(CC) $(OPTFLAGS) -c -o erfinv.o $(SRC_PATH)erfinv.c

is_point_close_or_off_the_frame_edge.o: $(SRC_PATH)is_point_close_or_off_the_frame_edge.c
	$(CC) $(OPTFLAGS) -c -o is_point_close_or_off_the_frame_edge.o $(SRC_PATH)is_point_close_or_off_the_frame_edge.c

guess_saturation_limit_main.o: $(SRC_PATH)guess_saturation_limit_main.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)guess_saturation_limit_main.c
#lib/guess_saturation_limit_main: guess_saturation_limit_main.o guess_saturation_limit.o autodetect_aperture.o exclude_region.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o
#	$(CC) $(OPTFLAGS) -o lib/guess_saturation_limit_main  guess_saturation_limit_main.o guess_saturation_limit.o autodetect_aperture.o exclude_region.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o $(GSL_LIB) $(CFITSIO_LIB) -lm
lib/guess_saturation_limit_main: guess_saturation_limit_main.o guess_saturation_limit.o vast_utils.o exclude_region.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o
	$(CC) $(OPTFLAGS) -o lib/guess_saturation_limit_main  guess_saturation_limit_main.o guess_saturation_limit.o vast_utils.o exclude_region.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o $(GSL_LIB) $(CFITSIO_LIB) -lm

lib/shutterless_bad_regions_hack: $(SRC_PATH)shutterless_bad_regions_hack.c
	$(CC) $(OPTFLAGS) -o lib/shutterless_bad_regions_hack $(SRC_PATH)shutterless_bad_regions_hack.c $(CFITSIO_LIB) -lm

variability_indexes.o: $(SRC_PATH)variability_indexes.c $(SRC_PATH)vast_limits.h $(SRC_PATH)variability_indexes.h
	# Older GCC versions complain about isnormal() unless -std=c99 is given explicitly
	#$(CC) $(OPTFLAGS) -c -o variability_indexes.o $(SRC_PATH)variability_indexes.c -std=c99 -I$(GSL_INCLUDE)
	# MacOS header files are incompatible with -std=c99
	$(CC) $(OPTFLAGS) -c -o variability_indexes.o $(SRC_PATH)variability_indexes.c -I$(GSL_INCLUDE)
create_data.o: $(SRC_PATH)create_data.c $(SRC_PATH)vast_limits.h $(SRC_PATH)variability_indexes.h $(SRC_PATH)get_number_of_measured_images_from_vast_summary_log.h $(SRC_PATH)detailed_error_messages.h $(SRC_PATH)lightcurve_io.h
	# Older GCC versions complain about isnormal() unless -std=c99 is given explicitly
	#$(CC) $(OPTFLAGS) -c -o create_data.o $(SRC_PATH)create_data.c -std=c99 -I$(GSL_INCLUDE)
	# MacOS header files are incompatible with -std=c99
	$(CC) $(OPTFLAGS) -c -o create_data.o $(SRC_PATH)create_data.c -I$(GSL_INCLUDE)
write_vast_lightcurve_statistics_format_log.o: $(SRC_PATH)write_vast_lightcurve_statistics_format_log.c
	$(CC) $(OPTFLAGS) -c -o write_vast_lightcurve_statistics_format_log.o $(SRC_PATH)write_vast_lightcurve_statistics_format_log.c
lib/create_data: create_data.o get_number_of_measured_images_from_vast_summary_log.o variability_indexes.o write_vast_lightcurve_statistics_format_log.o
	$(CC) $(OPTFLAGS) -o lib/create_data create_data.o get_number_of_measured_images_from_vast_summary_log.o variability_indexes.o write_vast_lightcurve_statistics_format_log.o $(GSL_LIB) -lm
lib/fast_clean_data: $(SRC_PATH)fast_clean_data.c
	$(CC) $(OPTFLAGS) -o lib/fast_clean_data $(SRC_PATH)fast_clean_data.c

split_multiextension_fits.o: $(SRC_PATH)split_multiextension_fits.c
	$(CC) $(OPTFLAGS) -c -o split_multiextension_fits.o $(SRC_PATH)split_multiextension_fits.c

util/split_multiextension_fits: split_multiextension_fits.o gettime.o kourovka_sbg_date.o
	$(CC) $(OPTFLAGS) -o util/split_multiextension_fits split_multiextension_fits.o gettime.o kourovka_sbg_date.o $(CFITSIO_LIB) -lm

formater_out_wfk: $(SRC_PATH)formater_out_wfk.c
	$(CC) $(OPTFLAGS) -o $(LIB_DIR)formater_out_wfk $(SRC_PATH)formater_out_wfk.c
stat_outfile: $(SRC_PATH)stat_outfile.c
	$(CC) $(OPTFLAGS) -o util/stat_outfile $(SRC_PATH)stat_outfile.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm

colstat.o: $(SRC_PATH)colstat.c
	$(CC) $(OPTFLAGS) -c -o colstat.o $(SRC_PATH)colstat.c -I$(GSL_INCLUDE)
util/colstat: colstat.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o util/colstat colstat.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm

imstat_vast.o: $(SRC_PATH)imstat_vast.c
	$(CC) $(OPTFLAGS) -c -o imstat_vast.o $(SRC_PATH)imstat_vast.c -I$(GSL_INCLUDE)
util/imstat_vast: imstat_vast.o variability_indexes.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o
	$(CC) $(OPTFLAGS) -o util/imstat_vast imstat_vast.o variability_indexes.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	cd util/ ; ln -s imstat_vast imstat_vast_fast ; cd ..


simulate_2_colors: $(SRC_PATH)simulate_2_colors.c
	$(CC) $(OPTFLAGS) -o $(LIB_DIR)simulate_2_colors $(SRC_PATH)simulate_2_colors.c

lib/try_to_guess_image_fov: $(SRC_PATH)try_to_guess_image_fov.c
	$(CC) $(OPTFLAGS) -o lib/try_to_guess_image_fov $(SRC_PATH)try_to_guess_image_fov.c $(CFITSIO_LIB) -lm

m_sigma_bin.o: $(SRC_PATH)m_sigma_bin.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)m_sigma_bin.c -I$(GSL_INCLUDE)

index_vs_mag.o: $(SRC_PATH)index_vs_mag.c
	#$(CC) $(OPTFLAGS) -c $(SRC_PATH)index_vs_mag.c -std=c99 -I$(GSL_INCLUDE)
	# MacOS header files are incompatible with -std=c99
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)index_vs_mag.c -I$(GSL_INCLUDE)

m_sigma_bin: m_sigma_bin.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o $(LIB_DIR)m_sigma_bin m_sigma_bin.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm

index_vs_mag: index_vs_mag.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o $(LIB_DIR)index_vs_mag index_vs_mag.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm



rescale_photometric_errors.o: $(SRC_PATH)rescale_photometric_errors.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)rescale_photometric_errors.c -I$(GSL_INCLUDE) -lm
rescale_photometric_errors: rescale_photometric_errors.o wpolyfit.o
	$(CC) $(OPTFLAGS) -o util/rescale_photometric_errors rescale_photometric_errors.o wpolyfit.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	cd util/ ; ln -s rescale_photometric_errors estimate_systematic_noise_level ; cd ..


select_sysrem_input_star_list: $(SRC_PATH)select_sysrem_input_star_list.c
	$(CC) $(OPTFLAGS) -o $(LIB_DIR)select_sysrem_input_star_list $(SRC_PATH)select_sysrem_input_star_list.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
drop: $(SRC_PATH)drop_faint_points.c $(SRC_PATH)drop_bright_points.c
	$(CC) $(OPTFLAGS) -o lib/drop_faint_points $(SRC_PATH)drop_faint_points.c
	$(CC) $(OPTFLAGS) -o lib/drop_bright_points $(SRC_PATH)drop_bright_points.c
	
util/calibrate_magnitude_scale:  calibrate_magnitude_scale.o photocurve.o
	$(CC) $(OPTFLAGS) -o util/calibrate_magnitude_scale calibrate_magnitude_scale.o photocurve.o $(GSL_LIB) -lm
calibrate_magnitude_scale.o:
	 $(CC) $(OPTFLAGS) -c -o calibrate_magnitude_scale.o $(SRC_PATH)calibrate_magnitude_scale.c
photocurve.o: $(SRC_PATH)photocurve.c
	$(CC) $(OPTFLAGS) -c -o photocurve.o $(SRC_PATH)photocurve.c -I$(GSL_INCLUDE)

fit_plane_lin.o: $(SRC_PATH)fit_plane_lin.c
	$(CC) $(OPTFLAGS) -c -o fit_plane_lin.o $(SRC_PATH)fit_plane_lin.c -I$(GSL_INCLUDE)

lib/deg2hms: $(SRC_PATH)deg2hms.c
	$(CC) $(OPTFLAGS) -o lib/deg2hms $(SRC_PATH)deg2hms.c -lm
lib/hms2deg: $(SRC_PATH)hms2deg.c
	$(CC) $(OPTFLAGS) -o lib/hms2deg $(SRC_PATH)hms2deg.c
lib/coord_v_dva_slova: $(SRC_PATH)coord_v_dva_slova.c
	$(CC) $(OPTFLAGS) -o lib/coord_v_dva_slova $(SRC_PATH)coord_v_dva_slova.c
lib/fix_photo_log: $(SRC_PATH)fix_photo_log.c
	$(CC) $(OPTFLAGS) -o lib/fix_photo_log $(SRC_PATH)fix_photo_log.c
lib/put_two_sources_in_one_field: $(SRC_PATH)put_two_sources_in_one_field.c
	$(CC) $(OPTFLAGS) -o lib/put_two_sources_in_one_field $(SRC_PATH)put_two_sources_in_one_field.c -lm

select_only_n_random_points_from_set_of_lightcurves.o: $(SRC_PATH)select_only_n_random_points_from_set_of_lightcurves.c
	$(CC) $(OPTFLAGS) -c -o select_only_n_random_points_from_set_of_lightcurves.o $(SRC_PATH)select_only_n_random_points_from_set_of_lightcurves.c -I$(GSL_INCLUDE) -lm
lib/select_only_n_random_points_from_set_of_lightcurves: select_only_n_random_points_from_set_of_lightcurves.o get_dates_from_lightcurve_files_function.o
	$(CC) $(OPTFLAGS) -o lib/select_only_n_random_points_from_set_of_lightcurves select_only_n_random_points_from_set_of_lightcurves.o get_dates_from_lightcurve_files_function.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm

lib/new_lightcurve_sigma_filter: $(SRC_PATH)new_lightcurve_sigma_filter.c
	$(CC) $(OPTFLAGS) -o lib/new_lightcurve_sigma_filter $(SRC_PATH)new_lightcurve_sigma_filter.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm

select_aperture_with_smallest_scatter_for_each_object.o: $(SRC_PATH)select_aperture_with_smallest_scatter_for_each_object.c
	$(CC) $(OPTFLAGS) -c -o select_aperture_with_smallest_scatter_for_each_object.o $(SRC_PATH)select_aperture_with_smallest_scatter_for_each_object.c -I$(GSL_INCLUDE) -lm
lib/select_aperture_with_smallest_scatter_for_each_object: select_aperture_with_smallest_scatter_for_each_object.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o lib/select_aperture_with_smallest_scatter_for_each_object select_aperture_with_smallest_scatter_for_each_object.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm


lib/kwee-van-woerden: $(SRC_PATH)kwee-van-woerden.c
	$(CC) $(OPTFLAGS) -o lib/kwee-van-woerden $(SRC_PATH)kwee-van-woerden.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm

on_the_fly_symlink_or_convert.o: $(SRC_PATH)on_the_fly_symlink_or_convert.c
	$(CC) $(OPTFLAGS) -c -o on_the_fly_symlink_or_convert.o $(SRC_PATH)on_the_fly_symlink_or_convert.c
lib/on_the_fly_symlink_or_convert: on_the_fly_symlink_or_convert.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o
	$(CC) $(OPTFLAGS) -o lib/on_the_fly_symlink_or_convert on_the_fly_symlink_or_convert.o replace_file_with_symlink_if_filename_contains_white_spaces.o get_path_to_vast.o $(CFITSIO_LIB) -lm

read_tycho2.o: $(SRC_PATH)catalogs/read_tycho2.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)catalogs/read_tycho2.c -I$(GSL_INCLUDE)
read_tycho2_main.o: $(SRC_PATH)catalogs/read_tycho2_main.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)catalogs/read_tycho2_main.c
create_tycho2_list_of_bright_stars_to_exclude_from_transient_search.o: $(SRC_PATH)catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search.c
lib/catalogs/read_tycho2: read_tycho2.o read_tycho2_main.o
	$(CC) $(OPTFLAGS) -o lib/catalogs/read_tycho2 read_tycho2.o read_tycho2_main.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm
lib/catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search: read_tycho2.o create_tycho2_list_of_bright_stars_to_exclude_from_transient_search.o
	$(CC) $(OPTFLAGS) -o lib/catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search read_tycho2.o create_tycho2_list_of_bright_stars_to_exclude_from_transient_search.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm

lib/catalogs/check_catalogs_offline: $(SRC_PATH)catalogs/check_catalogs_offline.c
	$(CC) $(OPTFLAGS) -o lib/catalogs/check_catalogs_offline $(SRC_PATH)catalogs/check_catalogs_offline.c -lm
util/get_image_date: get_image_date.o gettime.o kourovka_sbg_date.o
	$(CC) $(OPTFLAGS) -o util/get_image_date get_image_date.o gettime.o kourovka_sbg_date.o $(CFITSIO_LIB) -lm
	cd util/ ; ln -s get_image_date fix_image_date ; cd -
lib/find_flares: $(SRC_PATH)find_flares.c
	$(CC) $(OPTFLAGS) -o lib/find_flares $(SRC_PATH)find_flares.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
lib/autodetect_aperture_main: autodetect_aperture_main.o autodetect_aperture.o vast_utils.o guess_saturation_limit.o exclude_region.o gettime.o kourovka_sbg_date.o get_number_of_cpu_cores.o get_path_to_vast.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o
	$(CC) $(OPTFLAGS) -o lib/autodetect_aperture_main autodetect_aperture_main.o autodetect_aperture.o vast_utils.o guess_saturation_limit.o exclude_region.o gettime.o kourovka_sbg_date.o get_number_of_cpu_cores.o get_path_to_vast.o variability_indexes.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE)  -lm
	cd lib ; ln -s autodetect_aperture_main sextract_single_image_noninteractive ; cd ..
autodetect_aperture_main.o: $(SRC_PATH)autodetect_aperture_main.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)autodetect_aperture_main.c


remove_bad_images.o: $(SRC_PATH)remove_bad_images.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)remove_bad_images.c -I$(GSL_INCLUDE)
	
lib/remove_bad_images: remove_bad_images.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o lib/remove_bad_images remove_bad_images.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm $(OPTFLAGS)

lib/sort_all_lightcurve_files_in_jd: $(SRC_PATH)sort_all_lightcurve_files_in_jd.c
	$(CC) $(OPTFLAGS) -o lib/sort_all_lightcurve_files_in_jd $(SRC_PATH)sort_all_lightcurve_files_in_jd.c

MagSize_filter_standalone.o: $(SRC_PATH)MagSize_filter_standalone.c
	$(CC) $(OPTFLAGS) -c $(SRC_PATH)MagSize_filter_standalone.c -I$(GSL_INCLUDE)

lib/MagSize_filter_standalone: MagSize_filter_standalone.o filter_MagSize.o erfinv.o variability_indexes.o
	$(CC) $(OPTFLAGS) -o lib/MagSize_filter_standalone MagSize_filter_standalone.o filter_MagSize.o erfinv.o variability_indexes.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm


get_image_date.o: $(SRC_PATH)get_image_date.c
	$(CC) $(OPTFLAGS) -c -o get_image_date.o $(SRC_PATH)get_image_date.c
		
lib/period_search/periodFilter/periodS2: $(SRC_PATH)period_search/periodFilter/periodS2.c
	$(CC) $(OPTFLAGS) $(SRC_PATH)period_search/periodFilter/periodS2.c -o lib/periodFilter/periodS2 -lm
lib/periodFilter/periodFilter: $(SRC_PATH)period_search/periodFilter/periodFilter.c
	$(CC) $(OPTFLAGS) $(SRC_PATH)period_search/periodFilter/periodFilter.c -o lib/periodFilter/periodFilter
lib/BLS/bls: $(SRC_PATH)period_search/BLS/bls.c
	$(CC) $(OPTFLAGS) -o lib/BLS/bls $(SRC_PATH)period_search/BLS/bls.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
lib/lk_compute_periodogram: lib/deeming_compute_periodogram
	cd lib/; ln -s  deeming_compute_periodogram lk_compute_periodogram ; ln -s deeming_compute_periodogram compute_periodogram_allmethods ; ln -s deeming_compute_periodogram ls_compute_periodogram ; cd ..
deeming_compute_periodogram.o: $(SRC_PATH)period_search/deeming_compute_periodogram.c
	$(CC) $(OPTFLAGS) -c -o deeming_compute_periodogram.o $(SRC_PATH)period_search/deeming_compute_periodogram.c -I$(GSL_INCLUDE) -lm
lib/deeming_compute_periodogram: deeming_compute_periodogram.o #get_number_of_cpu_cores.o
	$(CC) $(OPTFLAGS) -o lib/deeming_compute_periodogram deeming_compute_periodogram.o $(USE_SINCOS_OPITION) $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	
util/sysrem: util/sysrem2
	cd util/; ln -s sysrem2 sysrem ; cd ..

get_dates_from_lightcurve_files_function.o: $(SRC_PATH)get_dates_from_lightcurve_files_function.c
	$(CC) $(OPTFLAGS) -c -o get_dates_from_lightcurve_files_function.o $(SRC_PATH)get_dates_from_lightcurve_files_function.c
sysrem2.o: $(SRC_PATH)sysrem2.c
	$(CC) $(OPTFLAGS) -c -o sysrem2.o $(SRC_PATH)sysrem2.c -I$(GSL_INCLUDE) -lm
util/sysrem2: sysrem2.o variability_indexes.o get_dates_from_lightcurve_files_function.o get_number_of_cpu_cores.o
	$(CC) $(OPTFLAGS) -o util/sysrem2 sysrem2.o variability_indexes.o get_dates_from_lightcurve_files_function.o get_number_of_cpu_cores.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm

lib/lightcurve_simulator: $(SRC_PATH)lightcurve_simulator.c
	$(CC) $(OPTFLAGS) -o lib/lightcurve_simulator $(SRC_PATH)lightcurve_simulator.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
lib/noise_lightcurve_simulator: $(SRC_PATH)noise_lightcurve_simulator.c
	$(CC) $(OPTFLAGS) -o lib/noise_lightcurve_simulator $(SRC_PATH)noise_lightcurve_simulator.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	
util/local_zeropoint_correction: $(SRC_PATH)local_zeropoint_correction.c
	$(CC) $(OPTFLAGS) -o util/local_zeropoint_correction $(SRC_PATH)local_zeropoint_correction.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
lib/checkstar: $(SRC_PATH)checkstar.c
	$(CC) $(OPTFLAGS) -o lib/checkstar $(SRC_PATH)checkstar.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm

lib/fit_parabola_wpolyfit: fit_parabola_wpolyfit.o wpolyfit.o
	$(CC) $(OPTFLAGS) -o lib/fit_parabola_wpolyfit fit_parabola_wpolyfit.o wpolyfit.o $(GSL_LIB) -I$(GSL_INCLUDE) -lm
fit_parabola_wpolyfit.o: $(SRC_PATH)fit_parabola_wpolyfit.c
	$(CC) $(OPTFLAGS) -c -o fit_parabola_wpolyfit.o $(SRC_PATH)fit_parabola_wpolyfit.c 
lib/remove_lightcurves_with_small_number_of_points: $(SRC_PATH)remove_lightcurves_with_small_number_of_points.c
	$(CC) $(OPTFLAGS) -o lib/remove_lightcurves_with_small_number_of_points $(SRC_PATH)remove_lightcurves_with_small_number_of_points.c
lib/transient_list: $(SRC_PATH)transient_list.c
	$(CC) $(OPTFLAGS) -o lib/transient_list $(SRC_PATH)transient_list.c
util/convert/CoRoT_FITS2ASCII: $(SRC_PATH)CoRoT_FITS2ASCII.c
	$(CC) $(OPTFLAGS) -o util/convert/CoRoT_FITS2ASCII $(SRC_PATH)CoRoT_FITS2ASCII.c $(CFITSIO_LIB) -lm
util/convert/SWASP_FITS2ASCII: $(SRC_PATH)SWASP_FITS2ASCII.c
	$(CC) $(OPTFLAGS) -o util/convert/SWASP_FITS2ASCII $(SRC_PATH)SWASP_FITS2ASCII.c $(CFITSIO_LIB) -lm
util/cute_lc: cute_lc.o vast_report_memory_error.o
	$(CC) $(OPTFLAGS) -o util/cute_lc cute_lc.o vast_report_memory_error.o $(GSL_LIB) -lm
	cd util/ ; ln -s cute_lc cute_lc_fullJD ; cd ..
cute_lc.o: $(SRC_PATH)cute_lc.c
	$(CC) $(OPTFLAGS) -c -o cute_lc.o $(SRC_PATH)cute_lc.c -I$(GSL_INCLUDE)
util/observations_per_star: $(SRC_PATH)observations_per_star.c
	$(CC) $(OPTFLAGS) -o util/observations_per_star $(SRC_PATH)observations_per_star.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
util/phase_lc: $(SRC_PATH)phase_lc.c
	$(CC) $(OPTFLAGS) -o util/phase_lc $(SRC_PATH)phase_lc.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	cd util/ ; ln -s phase_lc phase_and_bin_lc ; cd ..

util/UTC2TT: UTC2TT.o gettime.o kourovka_sbg_date.o
	$(CC) $(OPTFLAGS) -o util/UTC2TT UTC2TT.o gettime.o kourovka_sbg_date.o $(CFITSIO_LIB) -lm
	ln -s UTC2TT util/TT2UTC
	
UTC2TT.o: $(SRC_PATH)heliocentric_correction/UTC2TT.c
	$(CC) $(OPTFLAGS) -c -o UTC2TT.o $(SRC_PATH)heliocentric_correction/UTC2TT.c 

util/hjd: eph_manager.o hjd.o hjd_N.o novas.o novascon.o nutation.o readeph0.o solsys3.o gettime.o kourovka_sbg_date.o
	$(CC) $(OPTFLAGS) -o util/hjd hjd.o eph_manager.o hjd_N.o novas.o novascon.o nutation.o readeph0.o solsys3.o gettime.o kourovka_sbg_date.o $(CFITSIO_LIB) -lm
	ln -s hjd util/hjd_input_in_UTC
	ln -s hjd util/hjd_input_in_TT
eph_manager.o: $(SRC_PATH)heliocentric_correction/eph_manager.c
	$(CC) $(OPTFLAGS) -c -o eph_manager.o $(SRC_PATH)heliocentric_correction/eph_manager.c
hjd.o: $(SRC_PATH)heliocentric_correction/hjd.c
	$(CC) $(OPTFLAGS) -c -o hjd.o $(SRC_PATH)heliocentric_correction/hjd.c
hjd_N.o: $(SRC_PATH)heliocentric_correction/hjd_N.c
	$(CC) $(OPTFLAGS) -c -o hjd_N.o $(SRC_PATH)heliocentric_correction/hjd_N.c
novas.o: $(SRC_PATH)heliocentric_correction/novas.c
	$(CC) $(OPTFLAGS) -c -o novas.o $(SRC_PATH)heliocentric_correction/novas.c
novascon.o: $(SRC_PATH)heliocentric_correction/novascon.c
	$(CC) $(OPTFLAGS) -c -o novascon.o $(SRC_PATH)heliocentric_correction/novascon.c
nutation.o: $(SRC_PATH)heliocentric_correction/nutation.c
	$(CC) $(OPTFLAGS) -c -o nutation.o $(SRC_PATH)heliocentric_correction/nutation.c
readeph0.o: $(SRC_PATH)heliocentric_correction/readeph0.o
	$(CC) $(OPTFLAGS) -c -o readeph0.o $(SRC_PATH)heliocentric_correction/readeph0.c
solsys3.o: $(SRC_PATH)heliocentric_correction/solsys3.c
	$(CC) $(OPTFLAGS) -c -o solsys3.o $(SRC_PATH)heliocentric_correction/solsys3.c

lib/astrometry/get_image_dimentions: $(SRC_PATH)astrometry/get_image_dimentions.c
	$(CC) $(OPTFLAGS) -o lib/astrometry/get_image_dimentions $(SRC_PATH)astrometry/get_image_dimentions.c $(CFITSIO_LIB) -lm
lib/astrometry/insert_wcs_header: insert_wcs_header.o gettime.o kourovka_sbg_date.o
	$(CC) $(OPTFLAGS) -o lib/astrometry/insert_wcs_header insert_wcs_header.o gettime.o kourovka_sbg_date.o $(CFITSIO_LIB) -lm
lib/astrometry/strip_wcs_keywords: $(SRC_PATH)astrometry/strip_wcs_keywords.c
	$(CC) $(OPTFLAGS) -o lib/astrometry/strip_wcs_keywords $(SRC_PATH)astrometry/strip_wcs_keywords.c $(CFITSIO_LIB) -lm

insert_wcs_header.o: $(SRC_PATH)astrometry/insert_wcs_header.c
	$(CC) $(OPTFLAGS) -c -o insert_wcs_header.o $(SRC_PATH)astrometry/insert_wcs_header.c

lib/find_star_in_wcs_catalog: $(SRC_PATH)find_star_in_wcs_catalog.c
	 $(CC) $(OPTFLAGS) -o lib/find_star_in_wcs_catalog $(SRC_PATH)find_star_in_wcs_catalog.c -lm

make_outxyls_for_astrometric_calibration.o: $(SRC_PATH)make_outxyls_for_astrometric_calibration.c
	$(CC) $(OPTFLAGS) -c -o make_outxyls_for_astrometric_calibration.o $(SRC_PATH)make_outxyls_for_astrometric_calibration.c -I$(GSL_INCLUDE)
lib/make_outxyls_for_astrometric_calibration: make_outxyls_for_astrometric_calibration.o is_point_close_or_off_the_frame_edge.o
	$(CC) $(OPTFLAGS) -o lib/make_outxyls_for_astrometric_calibration make_outxyls_for_astrometric_calibration.o is_point_close_or_off_the_frame_edge.o $(GSL_LIB) -I$(GSL_INCLUDE) $(CFITSIO_LIB) -lm

util/solve_plate_with_UCAC5: solve_plate_with_UCAC5.o gettime.o kourovka_sbg_date.o wpolyfit.o variability_indexes.o get_path_to_vast.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o
	$(CC) $(OPTFLAGS) -o util/solve_plate_with_UCAC5 solve_plate_with_UCAC5.o gettime.o kourovka_sbg_date.o fit_plane_lin.o wpolyfit.o variability_indexes.o get_path_to_vast.o is_point_close_or_off_the_frame_edge.o replace_file_with_symlink_if_filename_contains_white_spaces.o $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE) -lm
	cd util/ ; ln -s solve_plate_with_UCAC5 solve_plate_with_UCAC4 ; cd ..
solve_plate_with_UCAC5.o:
	$(CC) $(OPTFLAGS) -c  $(SRC_PATH)solve_plate_with_UCAC5.c -I$(GSL_INCLUDE)

util/bin_lightcurve_in_time: $(SRC_PATH)bin_lightcurve_in_time.c
	$(CC) $(OPTFLAGS) -o util/bin_lightcurve_in_time $(SRC_PATH)bin_lightcurve_in_time.c $(GSL_LIB) -I$(GSL_INCLUDE) -lm

lib/ConstellationBoundaries:  $(SRC_PATH)catalogs/ConstellationBoundaries.c
	$(CC) $(OPTFLAGS) -o lib/ConstellationBoundaries $(SRC_PATH)catalogs/ConstellationBoundaries.c

#check_no_for_loop_initial_declaration:
#	(find . -type f -name "*.c" -exec grep -H -E "for[[:space:]]*\([[:space:]]*(int|size_t)" {} \; && echo "I don't like 'for' loop initial declarations! Please declare variables at the start of the function." && exit 1)
#check_no_for_loop_initial_declaration:
#	@if find . -type f -name "*.c" -exec grep -q -E "for[[:space:]]*\([[:space:]]*(int|size_t)" {} \+; then \
#		find . -type f -name "*.c" -exec grep -H -E "for[[:space:]]*\([[:space:]]*(int|size_t)" {} \+; \
#		echo "I don't like 'for' loop initial declarations! Please declare variables at the start of the function."; \
#		exit 1; \
#	fi
check_no_for_loop_initial_declaration:
	lib/check_no_for_loop_initial_declaration.sh	
	
shell_commands: pgplot_components lib/lightcurve_simulator vast
	ln -s vast diffphot
	ln -s pgfv sextract_single_image
	ln -s pgfv select_star_on_reference_image
	ln -s ../pgfv util/make_finding_chart
	ln -s ../pgfv util/make_finder_chart
	ln -s ../pgfv util/fits2png
	ln -s draw_stars_with_ds9.sh util/mark_wcs_position_with_ds9.sh
	cd util && ln -s identify.sh identify_noninteractive.sh && ln -s identify.sh identify_justname.sh && ln -s identify.sh wcs_image_calibration.sh && ln -s identify.sh identify_transient.sh && ln -s identify.sh identify_for_catalog.sh && cd -
	#
	cd lib && ln -s lightcurve_simulator sine_wave_simulator && ln -s lightcurve_simulator sine_wave_and_psd_simulator && ln -s lightcurve_simulator sine_wave_or_psd_simulator && cd -
	#
	cd lib && ln -s deg2hms deg2hms_uas && cd -
	#
	cd lib && ln -s autodetect_aperture_main fits2cat && cd -
	#
	cd lib && ln -s test_libpng.sh test_libpng_justtest_nomovepgplot.sh && cd -
	#
	cd util/artificial_star_test_for_transient_search && ln -s run_artificial_star_test.sh run_artificial_star_test_oneflux.sh && cd -
	#
	# This is to remove docs
	rm -f `find src/ -name '*.pdf'` `find src/ -name '*.ps'`
	#
	
shell_commands_record_compiler_version:
	$(CC) --version | head -n 1 > .cc.version # save compiler version
	LANG=C date > .cc.date # seve compile time
	lib/record_vast_build_number.sh # seve vast build number to .cc.build
	
libident.o: $(SOURCE_IDENT_PATH)ident_lib.c $(SOURCE_IDENT_PATH)ident.h 
	$(CC) $(OPTFLAGS) -c -o $(SRC_PATH)ident_lib.o -fPIC $(SOURCE_IDENT_PATH)ident_lib.c 
	cp $(SRC_PATH)ident_lib.o libident.o # file name kept for historical reasons

ccd: util/ccd/mk util/ccd/ms util/ccd/md util/ccd/mk_sigma_clip util/ccd/imgbin2x2

util/ccd/imgbin2x2: $(SRC_PATH)ccd/imgbin2x2.c $(SRC_PATH)ccd/imgbin2x2.py
	$(CC) $(OPTFLAGS) -o util/ccd/imgbin2x2 $(SRC_PATH)ccd/imgbin2x2.c $(CFITSIO_LIB) -lm
	cp $(SRC_PATH)ccd/imgbin2x2.py util/ccd/imgbin2x2.py
util/ccd/mk: $(SRC_PATH)ccd/mk.c
	$(CC) $(OPTFLAGS) -o util/ccd/mk $(SRC_PATH)ccd/mk.c $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE) -lm
util/ccd/mk_sigma_clip: $(SRC_PATH)ccd/mk_sigma_clip.c
	$(CC) $(OPTFLAGS) -o util/ccd/mk_sigma_clip $(SRC_PATH)ccd/mk_sigma_clip.c $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE) -lm
util/ccd/ms: $(SRC_PATH)ccd/ms.c
	$(CC) $(OPTFLAGS) -o util/ccd/ms $(SRC_PATH)ccd/ms.c $(CFITSIO_LIB) -lm
util/ccd/md: $(SRC_PATH)ccd/md.c
	$(CC) $(OPTFLAGS) -o util/ccd/md $(SRC_PATH)ccd/md.c $(CFITSIO_LIB) $(GSL_LIB) -I$(GSL_INCLUDE) -lm

ifneq ($(RECOMPILE_VAST_ONLY),yes)
clean_libraries:
	util/clean_data.sh all # remove all data (lightcurves, etc) from the previous VaST run
	rm -f vast libident.so libident.o $(LIB_DIR)stat $(LIB_DIR)formater_out_wfk stat_outfile $(LIB_DIR)simulate_2_colors combine_obs_median $(SRC_PATH)*.o $(SRC_PATH)*.so lc find_candidates pgfv $(SRC_PATH)pgfv/*.o $(SRC_PATH)diferential/*.o vast poisk util/stat_outfile util/combine_obs_median $(LIB_DIR)m_sigma_bin $(LIB_DIR)select_sysrem_input_star_list lib/periodFilter/periodS2 lib/periodFilter/periodFilter lib/BLS/bls util/ccd/*
	rm -f lib/libcfitsio.a lib/libz.a
	rm -f util/fitscopy util/funpack src/cfitsio/fitscopy src/fitsio.h src/longnam.h
	rm -f lib/fitsverify
	lib/compile_zlib.sh clean
	lib/compile_cfitsio.sh clean
	lib/compile_gsl.sh clean
	lib/compile_sextractor.sh clean
	lib/compile_astcheck.sh clean
	lib/compile_cdsclient.sh clean
	rm -f lib/bin/* 
	rm -rf lib/lib/*
	rm -rf lib/man/*
	rm -rf lib/include/*
	rm -rf lib/share/*
	rm -f lib/astcheck 
else
clean_libraries:
	# do nothing
endif

clean: clean_libraries
	rm -f *.o 
	rm -f callgrind.out.* # remove files from possible callgrind/kcachegrind profiling run: valgrind --tool=callgrind -v  ./vast -uf ../sample_data/f_72-00* ../sample_data/f_72-01*
	rm -f massif.out.* # same for the other valgrind tool
	rm -f *~ util/*~ util/transients/*~ lib/*~ lib/drop_faint_points lib/drop_bright_points DEADJOE tmp.txt match.txt  util/calibrate_magnitude_scale lib/fit_mag_calib lib/fit_linear lib/fit_robust_linear lib/fit_zeropoint lib/fit_photocurve lib/select_comparison_stars util/match_eater 
	rm -f lib/deg2hms lib/coord_v_dva_slova lib/hms2deg $(SRC_PATH)period_search/BLS/*~ $(SRC_PATH)period_search/periodFilter/*~ lib/fix_photo_log util/sysrem util/sysrem2 lib/lightcurve_simulator lib/noise_lightcurve_simulator util/local_zeropoint_correction lib/checkstar lib/put_two_sources_in_one_field lib/new_lightcurve_sigma_filter lib/data_parser lib/fit_parabola_wpolyfit lib/remove_lightcurves_with_small_number_of_points lib/transient_list lib/select_aperture_with_smallest_scatter_for_each_object util/hjd sextract_single_image diffphot select_star_on_reference_image util/mark_wcs_position_with_ds9.sh
	rm -f lib/deg2hms_uas
	rm -f lib/sine_wave_simulator lib/sine_wave_and_psd_simulator lib/sine_wave_or_psd_simulator
	rm -f util/rescale_photometric_errors util/estimate_systematic_noise_level
	rm -f util/colstat
	rm -f util/imstat_vast util/imstat_vast_fast
	rm -f src/*~
	rm -f util/convert/CoRoT_FITS2ASCII util/convert/SWASP_FITS2ASCII util/cute_lc util/cute_lc_fullJD util/observations_per_star lib/astrometry/get_image_dimentions lib/astrometry/insert_wcs_header lib/astrometry/strip_wcs_keywords lib/astrometry/*~ lib/kwee-van-woerden  lib/find_star_in_wcs_catalog
	rm -f src/heliocentric_correction/*~ util/hjd_input_in_UTC util/hjd_input_in_TT util/UTC2TT util/TT2UTC util/make_finding_chart util/make_finder_chart util/fits2png lib/find_flares lib/catalogs/read_tycho2 lib/catalogs/create_tycho2_list_of_bright_stars_to_exclude_from_transient_search lib/catalogs/check_catalogs_offline util/get_image_date lib/make_outxyls_for_astrometric_calibration lib/fits2cat lib/create_data lib/fast_clean_data util/solve_plate_with_UCAC5 lib/autodetect_aperture_main lib/sextract_single_image_noninteractive
	rm -f util/solve_plate_with_UCAC4
	rm -f util/solve_plate_with_UCAC5
	rm -f src/catalogs/*~
	rm -f .cc.version
	rm -f .cc.date
	# we do not delete .cc.build 
	rm -f util/wcs_image_calibration.sh util/identify_transient.sh util/identify_for_catalog.sh util/identify_noninteractive.sh util/identify_justname.sh
	rm -f lib/pgplot/cpgdemo lib/pgplot/pgdemo* lib/pgplot/pgxwin_server lib/pgplot/pgplot.doc lib/pgplot/*.a lib/pgplot/*.so lib/pgplot/*~ lib/pgplot/grfont.dat
	rm -f lib/vizquery
	rm -f lib/try_to_guess_image_fov
	rm -f lib/test/stetson_test
	rm -f util/split_multiextension_fits
	rm -f lib/ls_compute_periodogram lib/lk_compute_periodogram lib/deeming_compute_periodogram lib/compute_periodogram_allmethods
	rm -f lib/guess_saturation_limit_main
	rm -f lib/shutterless_bad_regions_hack
	rm -f lib/remove_bad_images lib/sort_all_lightcurve_files_in_jd lib/MagSize_filter_standalone
	rm -f lib/select_only_n_random_points_from_set_of_lightcurves
	rm -f lib/index_vs_mag
	rm -f lib/on_the_fly_symlink_or_convert
	rm -f util/fix_image_date
	rm -f util/phase_lc util/phase_and_bin_lc
	rm -f util/bin_lightcurve_in_time
	rm -f lib/ConstellationBoundaries
	# symlink to cpgplot.h should be re-created by lib/compile_pgplot_related_components.sh
	rm -f src/cpgplot.h src/pgfv/cpgplot.h
	rm -f lib/test_libpng_justtest_nomovepgplot.sh
	#
	rm -f util/artificial_star_test_for_transient_search/run_artificial_star_test_oneflux.sh
	

clean_objects: vast statistics etc pgplot_components old shell_commands period_filter ccd astrometry astcheck cdsclient
	rm -f *.o $(SRC_PATH)*.o $(SRC_PATH)pgfv/*.o $(SRC_PATH)diferential/*.o  $(SRC_PATH)astrometry/*.o $(SRC_PATH)heliocentric_correction/*.o

print_check_start_message:
	@echo " "
	@echo -e "\033[01;34mChecking external programs...\033[00m"
	@echo " "

print_compile_start_message:
	@echo " "
	@echo -e "\033[01;34mCompiling VaST...\033[00m"
	@echo " "

print_compile_success_message: clean_objects sextractor astcheck cdsclient
	@echo " "
	@echo -e "\033[01;34mVaST seems to be compiled successfully! =)\033[00m"
	@echo " "
