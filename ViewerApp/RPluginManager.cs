using R3D.IOTools;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using Viewer.Infrastructure;
using Viewer.Interfaces;
using Viewer.Tools;

namespace ViewerApp
{
    class RPluginManager : IRPluginManager
    {
        AppData _data;

        List<string> _pluginPaths = new List<string>();

        public List<string> PluginPaths
        {
            get
            {
                return _pluginPaths;
            }

            set
            {
                _pluginPaths = value;
            }
        }

        public RPluginManager(AppData data)
        {
            _data = data;

            ActivatePlugin(typeof(InternalToolsPlugin));
            ActivatePlugin(typeof(R3DIOToolsPlugin));
            CreateSamplePlugin();
        }

        public void CreateSamplePlugin()
        {
            string samplePluginExpectedPath = Path.Combine(ConstantNames.PluginDirectory, ConstantNames.SamplePluginName);

            if (!File.Exists(samplePluginExpectedPath))
            {
                if (!Directory.Exists(Path.GetDirectoryName(samplePluginExpectedPath)))
                    Directory.CreateDirectory(Path.GetDirectoryName(samplePluginExpectedPath));

                File.Copy(Path.Combine(_data.AssemblyDirectory, ConstantNames.SamplePluginName), samplePluginExpectedPath);
            }
        }

        public void LoadExternalPlugins()
        {
            var pluginPaths = GetPluginAssemblyFullPaths();

            foreach (var item in pluginPaths)
            {
                string assemblyName = item;
                byte[] assemblyBytes = File.ReadAllBytes(assemblyName);
                Assembly assembly = Assembly.Load(assemblyBytes);

                foreach (Type type in assembly.GetTypes())
                {
                    System.Attribute[] attributes = System.Attribute.GetCustomAttributes(type);

                    foreach (Attribute attribute in attributes)
                    {
                        if (attribute is RPluginAttribute)
                        {
                            _pluginPaths.Add(item);
                            ActivatePlugin(type);
                        }
                    }
                }
            }
          _pluginPaths =  _pluginPaths.Distinct().ToList();
        }


        public List<string> GetPluginAssemblyFullPaths()
        {
            DirectoryInfo ii = new DirectoryInfo(ConstantNames.PluginDirectory);
            var files = ii.EnumerateFiles("*" + ConstantNames.PluginExtension);

            List<string> dlls = new List<string>();

            foreach (var item in files)
            {
                FileStream file = item.OpenRead();

                XmlDataDocument xmldoc = new XmlDataDocument();
                XmlNodeList xmlnode;
                int i = 0;
                string str = null;
                xmldoc.Load(file);
                xmlnode = xmldoc.GetElementsByTagName(ConstantNames.PluginTag);
                for (i = 0; i <= xmlnode.Count - 1; i++)
                {
                    for (int j = 0; j < xmlnode[i].Attributes.Count; j++)
                    {
                        var attr = xmlnode[i]?.Attributes[j];

                        if (attr?.Name == "Path")
                        {
                            if (File.Exists(attr.Value))
                                dlls.Add(attr.Value);
                        }
                    }
                }
            }

            return dlls;
        }

        public void ActivatePlugin(Type t)
        {
            var instance = (IRPlugin)Activator.CreateInstance(t);
            instance.Connect(_data);
            _data.Plugins.Add(instance);
            ResolveXamlResources(instance);

            var documen = instance.GetDocumentData();
            if (documen == null)
                return;
            _data.DocumentTypeData.AddRange(documen);

        }

        private void ResolveXamlResources(IRPlugin plugin)
        {
            string resourcePath = plugin.GetXAMLResourcePath();
            if (string.IsNullOrEmpty(resourcePath))
                return;
            ResourceDictionary dic = new ResourceDictionary();

            dic.Source = new Uri(resourcePath, UriKind.RelativeOrAbsolute);

            System.Windows.Application.Current.Resources.MergedDictionaries.Add(dic);
        }



    }
}
