include(GNUInstallDirs)

# Generating the mo files.

# Add new language codes here, after "ITEMS".
foreach(lang IN ITEMS fi)
  set(infile  "${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po")
  set(outfile "${CMAKE_CURRENT_BINARY_DIR}/${lang}.mo")

  add_custom_command(OUTPUT "${outfile}" DEPENDS "${infile}"
    COMMAND msgfmt --check --statistics --output "${outfile}" "${infile}")

  install(FILES "${outfile}"
          DESTINATION "${CMAKE_INSTALL_LOCALEDIR}/${lang}/LC_MESSAGES"
          RENAME pong--11.mo)

  list(APPEND mo_files "${outfile}")
endforeach()

add_custom_target(translations ALL DEPENDS ${mo_files})

# Updating the pot file.

set(pot_file "${CMAKE_CURRENT_SOURCE_DIR}/pong--11.pot")
# Relative paths that will appear in .pot and .po files
set(src_list src/game.cpp src/main.cpp)

foreach(relative_path ${src_list})
  list(APPEND abs_src_list "${CMAKE_SOURCE_DIR}/${relative_path}")
endforeach()

add_custom_command(OUTPUT "${pot_file}" DEPENDS ${abs_src_list}
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  COMMAND xgettext --force-po --keyword=_ --boost
          --package-name=pong--11 --package-version=${PROJECT_VERSION}
          --output "${pot_file}" ${src_list})

add_custom_target(update_pot_file DEPENDS "${pot_file}")
