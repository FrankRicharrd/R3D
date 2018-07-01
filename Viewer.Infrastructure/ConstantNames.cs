using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Infrastructure
{
    public class ConstantNames
    {

        public static string ApplicationName = "R3D Engine";

        public static string IconFolderName = "Icons";


        public static string RBuilderDocIconName = "RBuilderDocIcon.png";


        public static string PluginExtension = ".r3dplugin";

        public static string PluginDirectory = System.IO.Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), ApplicationName, "Plugins");

        public static string SamplePluginName = "sampleplugin" + PluginExtension;

        public static string PluginTag = "R3DPlugin";

        public static string ShadersDirectory = CreateDirectory(Path.Combine(AssemblyDirectory, "ShaderPrograms"));

        public static string ShaderExtension = ".r3dshader";
        
        public static string AssemblyDirectory
        {
            get
            {
                string codeBase = Assembly.GetExecutingAssembly().CodeBase;
                UriBuilder uri = new UriBuilder(codeBase);
                string path = Uri.UnescapeDataString(uri.Path);
                return Path.GetDirectoryName(path);
            }
        }


        public static string CreateDirectory(string path)
        {

            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }

            return path;
        }


    }
}
