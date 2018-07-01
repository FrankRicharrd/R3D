using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Interfaces;
using R3DIOInterop;
using R3D.IOTools.IntelligentObjects;

using Viewer.Interfaces.IntelligentObject;
using System.Collections.ObjectModel;
using System.Windows.Input;
using ViewerCommon;
using System.Reflection;
using System.IO;

namespace R3D.IOTools.ViewModels
{

    public class IObjectBrowserViewModel : ToolViewModel
    {


        static ObservableCollection<IRIntelligentObjectPlugin> _intelligentObjects = new ObservableCollection<IRIntelligentObjectPlugin>();

        IRIntelligentObjectPlugin _selectedIobject;
        public IRIntelligentObjectPlugin SelectedIobject
        {
            get
            {
                return _selectedIobject;
            }
            set
            {
                _selectedIobject = value;
                RaisePropertyChanged();
            }
        }

        IApplicationData _data;
        IEventAggregator _event;
        IRBuilderDocument _doc;
        private RIntelligentObjectManagerInterop _manager;

        public ICommand IOSelectionChanged { get; set; }

        //get intelligentobjectmanager in this viewmodel and this class interact with the interop to create intelligent objects.
        public IObjectBrowserViewModel() : base("IntelligentObject")
        {

        }

        public override bool Connect(IApplicationData data, Prism.Events.IEventAggregator eventaggre)
        {
            _data = data;
            _event = eventaggre;
            _doc = data.ActiveDocument as IRBuilderDocument;


            IOSelectionChanged = new RelayCommand<object>(IOSelectionChangedHandler);

            _doc.SetSelectiontype(Viewer.SelectionType.IntelligentObject);
            _manager = new R3DIOInterop.RIntelligentObjectManagerInterop(_doc.Tree.GeometryParentNode);

            LoadObjectsFromPlugin();

            LoadInternalObjects();
            //CreateCube();

           // CreateTable();

            return true;
        }

        private void LoadInternalObjects()
        {
            RTable table = new RTable();
            table.Connect(_data, _manager);
            _intelligentObjects.Add(table);


            RBed b = new RBed();
            b.Connect(_data, _manager);
            _intelligentObjects.Add(b);
        }

        private void LoadObjectsFromPlugin()
        {
            foreach (var item in _data.PluginManager.PluginPaths)
            {
                string assemblyName = item;
                byte[] assemblyBytes = File.ReadAllBytes(assemblyName);
                Assembly assembly = Assembly.Load(assemblyBytes);

                foreach (Type type in assembly.GetTypes())
                {
                    System.Attribute[] attributes = System.Attribute.GetCustomAttributes(type);

                    foreach (Attribute attribute in attributes)
                    {
                        if (attribute is RIntelligentObjectAttribute)
                        {
                            var io = (IRIntelligentObjectPlugin)Activator.CreateInstance(type);
                            io.Connect(_data, _manager);
                            _intelligentObjects.Add(io);
                        }
                    }
                }
            }
        }

        private void IOSelectionChangedHandler(object selitem)
        {
            if (selitem == null)
                return;
            (selitem as IRIntelligentObjectPlugin).CreateObject();
        }

        RTable table;

        public ObservableCollection<IRIntelligentObjectPlugin> IntelligentObjects
        {
            get
            {
                return _intelligentObjects;
            }

            set
            {
                _intelligentObjects = value;

                RaisePropertyChanged();
            }
        }

        private void CreateTable()
        {
            table = new RTable();
            table.CreateTable(10, 5, 5, 0.25);
        }

        private void CreateCube()
        {
            R3DIOInterop.RIntelligentObjectManagerInterop objman = new R3DIOInterop.RIntelligentObjectManagerInterop(_doc.Tree.GeometryParentNode);

            //objman.CreateEmpytyObject();

            var intelliobj = objman.CreateIntelligentObject();

            RWoodPiece w = new RWoodPiece(intelliobj);
            w.DefineWoodPiece(10, 10, 10);
            w.CreateInstance(0, 0, 0);
            intelliobj.Redraw(true);

            //double thickness = 1;
            //double length = 1;
            //double width = 1;

            //RVertex3DInterop v0 = new RVertex3DInterop(0, 0, 0);
            //RVertex3DInterop v1 = new RVertex3DInterop(length, 0, 0);
            //RVertex3DInterop v2 = new RVertex3DInterop(length, thickness, 0);
            //RVertex3DInterop v3 = new RVertex3DInterop(0, thickness, 0);
            //RVertex3DInterop v4 = new RVertex3DInterop(0, 0, width);
            //RVertex3DInterop v5 = new RVertex3DInterop(length, 0, width);
            //RVertex3DInterop v6 = new RVertex3DInterop(length, thickness, width);
            //RVertex3DInterop v7 = new RVertex3DInterop(0, thickness, width);

            //List<RVertex3DInterop> lstvertices = new List<RVertex3DInterop>();
            //lstvertices.Add(v0);
            //lstvertices.Add(v1);
            //lstvertices.Add(v2);
            //lstvertices.Add(v3);
            //lstvertices.Add(v4);
            //lstvertices.Add(v5);
            //lstvertices.Add(v6);
            //lstvertices.Add(v7);



            //var definition = intelliobj.CreateIntelliDrawableDefinition(lstvertices);

            //int f1 = definition.CreateFace(new List<int>() { 4, 5, 6, 7 });
            //int f2 = definition.CreateFace(new List<int>() { 5, 1, 2, 6 });
            //int f3 = definition.CreateFace(new List<int>() { 1, 0, 3, 2 });
            //int f4 = definition.CreateFace(new List<int>() { 0, 4, 7, 3 });
            //int f5 = definition.CreateFace(new List<int>() { 7, 6, 2, 3 });
            //int f6 = definition.CreateFace(new List<int>() { 0, 1, 5, 4 });

            ////definition.AddFaceDragger(f1);
            ////definition.AddFaceDragger(f2);
            ////definition.AddFaceDragger(f3);
            ////definition.AddFaceDragger(f4);
            ////definition.AddFaceDragger(f5);
            ////definition.AddFaceDragger(f6);


            //var instance = definition.CreateInstance();

            //intelliobj.Redraw(true);
            ////definition.SetDraggerVisibility(true);

        }



        public override bool Disconnect()
        {
            _doc.SetSelectiontype(Viewer.SelectionType.TransformNode);
            return true;
        }
    }
}
