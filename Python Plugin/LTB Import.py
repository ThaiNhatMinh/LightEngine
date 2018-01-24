import bpy


bl_info = {
    "name": "LithTech Binary (.LTB)",
    "author": "Thai Nhat Minh",
    "version": (0, 0, 1),
    "blender": (2, 79, 0),
    "location": "File > Import-Export > LTB Model",
    "description": "LithTech Binary format (.md3)",
    "warning": "",
    "category": "Import-Export",
}

def read_some_data(context, filepath):
    print("running read_some_data...")
    f = open(filepath, 'r', encoding='utf-8')
    data = f.read()
    f.close()

    # would normally load the data here
    print(data)

    return {'FINISHED'}


# ImportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ImportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ImportLTB(Operator, ImportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "import_scene.LTB"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Import LTB"

    # ImportHelper mixin class uses this
    filename_ext = ".LTB"

    filter_glob = StringProperty(
            default="*.LTB",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )
    def execute(self, context):
        return read_some_data(context, self.filepath)


# Only needed if you want to add into a dynamic menu
def menu_func_import(self, context):
    self.layout.operator(ImportLTB.bl_idname, text="LithTech Binary (.LTB)")


def register():
    bpy.utils.register_class(ImportSomeData)
    bpy.types.INFO_MT_file_import.append(menu_func_import)


def unregister():
    bpy.utils.unregister_class(ImportSomeData)
    bpy.types.INFO_MT_file_import.remove(menu_func_import)


if __name__ == "__main__":
    register()

