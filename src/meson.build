cc = meson.get_compiler('cpp')
examples_dep = [thorvg_dep, dependency('sdl2', required: true)]
compiler_flags = ['-DSDL_MAIN_HANDLED']

# GL or GLES
gl_dep = dependency('gl', required: false)
gles_dep = dependency('glesv2', required: false)

if gl_dep.found()
    examples_dep += gl_dep
elif gles_dep.found()
    examples_dep += gles_dep
    compiler_flags += ['-DTVGEXAMPLE_GLES_SUPPORTED']
else
    message('Neither OpenGL nor OpenGL ES was found. Skipping GL examples.')
endif

# WebGPU
wgpu_dep = dependency('wgpu_native', required: false)

if wgpu_dep.found()
    examples_dep += wgpu_dep
    compiler_flags += ['-DTVGEXAMPLE_WGPU_SUPPORTED']
    if host_machine.system() == 'darwin'
        add_languages('objcpp', native: false)
        examples_dep += declare_dependency(link_args: ['-lobjc', '-framework', 'Cocoa', '-framework', 'CoreVideo', '-framework', 'IOKit', '-framework', 'QuartzCore'])
        compiler_flags += ['-x', 'objective-c++']
    endif
else
    message('wgpu_native not found, skipping WGPU examples.')
endif

source_file = [
    'Accessor.cpp',
    'Animation.cpp',
    'Blending.cpp',
    'BoundingBox.cpp',
    'Clipping.cpp',
    'CustomTransform.cpp',
    'DataLoad.cpp',
    'DirectUpdate.cpp',
    'Duplicate.cpp',
    'EffectDropShadow.cpp',
    'GifSaver.cpp',
    'SceneEffects.cpp',
    'FillRule.cpp',
    'Filling.cpp',
    'FillSpread.cpp',
    'FillSpreadSamplerReuse.cpp',
    'GradientMasking.cpp',
    'GradientStroke.cpp',
    'GradientTransform.cpp',
    'ImageRotation.cpp',
    'ImageScaling.cpp',
    'Intersects.cpp',
    'LinearGradient.cpp',
    'Masking.cpp',
    'MaskingMethods.cpp',
    'ManyRects.cpp',
    'MultiCanvas.cpp',
    'Opacity.cpp',
    'Path.cpp',
    'PictureJpg.cpp',
    'PicturePng.cpp',
    'PictureRaw.cpp',
    'PictureSvg.cpp',
    'PictureWebp.cpp',
    'RadialGradient.cpp',
    'Retaining.cpp',
    'Scene.cpp',
    'SceneBlending.cpp',
    'SceneTransform.cpp',
    'Shapes.cpp',
    'SmartRender.cpp',
    'Stroke.cpp',
    'StrokeLine.cpp',
    'StrokeMiterlimit.cpp',
    'Svg.cpp',
    'Text.cpp',
    'TextEffects.cpp',
    'TextLayout.cpp',
    'TextLineWrap.cpp',
    'Transform.cpp',
    'TrimPath.cpp',
    'Viewport.cpp',
]

thorvg_inc = thorvg_proj.get_variable('thorvg_inc')
thorvg_lottie_inc = include_directories(join_paths('..', 'subprojects', thorvg_subproject, 'src', 'loaders', 'lottie'))
thorvg_capi_inc = include_directories(join_paths('..', 'subprojects', thorvg_subproject, 'src', 'bindings', 'capi'))
extra_inc = [thorvg_inc, thorvg_lottie_inc, thorvg_capi_inc]

# Text metrics is still experimental and is not available in every comparison
# subproject used by the regression setup.
if cc.compiles('''
    #include <thorvg.h>

    void probe(tvg::Text* text)
    {
        tvg::TextMetrics textMetric;
        tvg::GlyphMetrics glyphMetric;
        (void) text->text();
        (void) text->lines();
        (void) text->metrics(textMetric);
        (void) text->metrics("a", glyphMetric);
    }
''',
    name: 'ThorVG text metrics API',
    include_directories: thorvg_inc)
    source_file += ['TextMetrics.cpp']
    message('ThorVG text metrics API found. Enabled TextMetrics example.')
else
    message('ThorVG text metrics API not found. Skipping TextMetrics example.')
endif

# Lottie Examples
if fs.is_file(join_paths(meson.current_source_dir(), '..', 'subprojects', thorvg_subproject, 'src', 'loaders', 'lottie', 'thorvg_lottie.h'))
    source_file += [
        'Lottie.cpp',
        'LottieExpressions.cpp',
        'LottieExtension.cpp',
        'LottieInteraction.cpp',
        'LottieBigSize.cpp',
        'LottieTweening.cpp',
        'DragDrop.cpp'
    ]
    message('ThorVG Lottie found. Enabled Lottie examples.')
else
    message('ThorVG Lottie not found. Skipping Lottie examples.')
endif

# CAPI Examples
if fs.is_file(join_paths(meson.current_source_dir(), '..', 'subprojects', thorvg_subproject, 'src', 'bindings', 'capi', 'thorvg_capi.h'))
    source_file += ['Capi.cpp']
    message('ThorVG CAPI found. Enabled Capi examples.')
else
    message('ThorVG CAPI not found. Skipping Capi examples.')
endif

foreach current_file : source_file
    name = current_file.split('.')[0]
    executable(name, current_file,
        include_directories: extra_inc,
        dependencies: examples_dep,
        cpp_args : compiler_flags)
endforeach

if cc.get_id() == 'msvc'
    configure_file(input: 'all.bat', output: 'all.bat', copy: true)
else
    configure_file(input: 'all.sh', output: 'all.sh', copy: true)
endif
