SolisVulkanSource = [
    'Buffer.cpp',
    'CommandBuffer.cpp',
    'DescriptorPool.cpp',
    'DescriptorSetLayout.cpp',
    'Device.cpp',
    'Driver.cpp',
    'Framebuffer.cpp',
    'GraphicsPipeline.cpp',
    'Image.cpp',
    'Instance.cpp',
    'Queue.cpp',
    'RenderPass.cpp',
    'Shader.cpp',
    'Swapchain.cpp',
    'Sync.cpp',
]

SolisVulkanLib = shared_library('SolisVulkanLib', SolisVulkanSource,
    name_prefix         : '',
    link_with           : [ ],
    dependencies        : [ lib_vulkan, lib_SDL2main, lib_SDL2 ],
    include_directories : [ include_path ],
    install             : true)

SolisVulkanDep = declare_dependency(
    link_with           : [ SolisVulkanLib ],
    include_directories : [ include_directories('.') ])
