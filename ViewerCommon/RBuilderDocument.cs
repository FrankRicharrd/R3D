using RExternalData;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using Viewer;
using Viewer.AdvancedTypes.Document;
using Viewer.Commands;
using Viewer.Infrastructure;
using Viewer.Interfaces;
using Viewer.Interfaces.IntelligentObject;
using Viewer.Interfaces.UI;
using Viewer.Tools.ViewModels;
using ViewerCommon.UI;
using ViewerGraphicsEditor;

namespace ViewerCommon
{

    public class RBuilderDocumentData : RDocumentData
    {
        public List<string> ShaderFiles { get; set; }
        public RBuilderDocumentData(string typeName, Type docType, string iconPath, Type viewModel) : base(typeName, docType, iconPath, viewModel)
        {
            ShaderFiles = new List<string>();

            LoadShaderFilesFromDirectory(ConstantNames.ShadersDirectory);
        }

        public void LoadShaderFilesFromDirectory(string directoryPath)
        {
            string[] arr = Directory.GetFiles(directoryPath);

            foreach (var file in arr)
            {
                if (file.EndsWith(ConstantNames.ShaderExtension))
                {
                    ShaderFiles.Add(file);
                }
            }
        }
    }


    public class RBuilderDocument : RDocument, IRBuilderDocument
    {
        ITree _tree;


        public ITree Tree
        {
            get
            {
                return _tree;
            }

            set
            {
                _tree = value;
            }
        }

        public IEditorInterop GraphicsEditor
        {
            get
            {
                return Editor;
            }

            set
            {
                Editor = value;
            }
        }

        public List<IRNode> SelectedNodes
        {
            get
            {
                return _selectedNodes;
            }

            set
            {
                _selectedNodes = value;
            }
        }

        List<IRNode> _selectedNodes = new List<IRNode>();

        ModelingCommands _modelerCommands;
        LightCommands _lightCommands;

        public RBuilderDocument(IApplicationData data) : base(data)
        {
            Editor = new ViewerInterop.ViewerEditor();
            Tree = new TreeData(Data, this);

            data.Eventaggregator.GetEvent<OnContextCreatedEvent>().Subscribe(OnContextCreated);

            _modelerCommands = new ModelingCommands(data, GraphicsEditor.GetModelingManager());
            _lightCommands = new LightCommands(data, GraphicsEditor.GetLightsManager());


        }

        private bool OnTransformNodeSelectedHandler(IRTransformNode node)
        {
            if (node == null) return false;
            Data.Eventaggregator.GetEvent<OnTransformNodeSelectedOnScreen>().Publish(node);
            return true;
        }

        private void OnContextCreated(IRNode obj)
        {
            ViewerInterop.ViewerEditor veditor = Editor as ViewerInterop.ViewerEditor;
            veditor.SubscribeEventForTransformNodeSelected(new ViewerInterop.OnTransformNodeSelected(OnTransformNodeSelectedHandler));
        }

        public void CreateRibbonPanel()
        {
            //UIManager = new RUIManager(Data);
            //// Data.CreateBasicRibbionPanel(UIManager);

            //var homeTab = UIManager.CreateNewTab("Home");

            //var buttonGroup = homeTab.CreateButtonGroup("Manipulators");

            //var button = buttonGroup.CreateToggleButton("Translate", UIManager.Data.GetIconPath("translate.png"), new RelayCommand<RRibbonToggleButton>(TranslateCommandhandler) );
            //var buttonro = buttonGroup.CreateToggleButton("Rotate", UIManager.Data.GetIconPath("rotate.png"), new RelayCommand<RRibbonToggleButton>(RotateCommandhandler));
            //var buttonsc = buttonGroup.CreateToggleButton("Scale", UIManager.Data.GetIconPath("scale.png"), new RelayCommand<RRibbonToggleButton>(ScaleCommandhandler));


        }

        public List<string> GetShaderPaths()
        {
            return (Data.DocumentTypeData.Where(a => a.DocType == this.GetType()).FirstOrDefault() as RBuilderDocumentData).ShaderFiles;
        }

        public void AddShaderPathToCurrentShaderLibrary(string path)
        {
            (Data.DocumentTypeData.Where(a => a.DocType == this.GetType()).FirstOrDefault() as RBuilderDocumentData).ShaderFiles.Add(path);
        }


        private void ScaleCommandhandler(RRibbonToggleButton ribbonToggleButton)
        {

            if (ribbonToggleButton.IsChecked == true)
                GraphicsEditor.SetDraggerState(DraggerType.TabBoxTrackballDragger, true);
            else
                GraphicsEditor.SetDraggerState(DraggerType.TabBoxTrackballDragger, false);
        }

        private void RotateCommandhandler(RRibbonToggleButton ribbonToggleButton)
        {

            if (ribbonToggleButton.IsChecked == true)
                GraphicsEditor.SetDraggerState(DraggerType.RotateSphereDragger, true);
            else
                GraphicsEditor.SetDraggerState(DraggerType.RotateSphereDragger, false);
        }

        private void TranslateCommandhandler(RRibbonToggleButton ribbonToggleButton)
        {

            if (ribbonToggleButton.IsChecked == true)
                GraphicsEditor.SetDraggerState(DraggerType.TranslateAxisDragger, true);
            else
                GraphicsEditor.SetDraggerState(DraggerType.TranslateAxisDragger, false);
        }

        public override void CreateDocumentSpecificRibbonTabs()
        {
            //if (Data.DocumentTypesLoaded.ContainsKey(this.GetType().ToString()))
            //    return;

            var homeTab = Data.UIManager.CreateNewTab(this.GetType().ToString(), "Home");

            var buttonGroup = homeTab.CreateButtonGroup("Manipulators");

            var button = buttonGroup.CreateToggleButton("Translate", Data.GetIconPath("translate.png"), new RelayCommand<RRibbonToggleButton>(TranslateCommandhandler));

            var buttonro = buttonGroup.CreateToggleButton("Rotate", Data.GetIconPath("rotate.png"), new RelayCommand<RRibbonToggleButton>(RotateCommandhandler));

            var buttonsc = buttonGroup.CreateToggleButton("Scale", Data.GetIconPath("scale.png"), new RelayCommand<RRibbonToggleButton>(ScaleCommandhandler));

            var buttonGroupImport = homeTab.CreateButtonGroup("ImportGeometry");

            var buttonimport = buttonGroupImport.CreateButton("Import", Data.GetIconPath("import.png"),
                new Viewer.Commands.GeometryImportCommandHandler(Data, Data.Eventaggregator));

            var buttonCustomimport = buttonGroupImport.CreateButton("Import_Custom_Data", Data.GetIconPath("import.png"),
                new RelayCommand(buttonCustomimportHandler));




            var modelingTab = Data.UIManager.CreateNewTab(this.GetType().ToString(), "Modeling");

            var meshmodelingbuttonGroup = modelingTab.CreateButtonGroup("MeshModeling");

            var spherebutton = meshmodelingbuttonGroup.CreateButton("Sphere", Data.GetIconPath("sphere.png"), new RelayCommand(_modelerCommands.CreateSphere));

            _lightCommands.CreateLightTab(this.GetType().ToString());

        }

        private void buttonCustomimportHandler()
        {
            try
            {
                OpenFileDialog openFileDialog1 = new OpenFileDialog();

                openFileDialog1.Title = "Browse Custom Geometry File";

                openFileDialog1.CheckFileExists = true;
                openFileDialog1.CheckPathExists = true;

                openFileDialog1.RestoreDirectory = true;

                if (openFileDialog1.ShowDialog() != DialogResult.OK)
                    return;



                XmlSerializer bf = new XmlSerializer(typeof(RevitData));

                FileStream fsout = new FileStream(openFileDialog1.FileName, FileMode.OpenOrCreate);

                RevitData ddata = null;

                using (fsout)
                {
                    ddata = (RevitData)bf.Deserialize(fsout);

                }

                if (ddata == null)
                {
                    MessageBox.Show("invalid format or deser failed.");
                    return;
                }

                IRModelingManager modelingManager = GetModelingManager();

                //foreach (var def in ddata.Definitions)
                //{
                //    var v = new List<double>();
                //    var i = new List<int>();
                //    def.Faces.ForEach((f) =>
                //    {
                //        f.Mesh.Vertices.ForEach((vi) =>
                //        {
                //            v.Add(vi.x);
                //            v.Add(vi.y);
                //            v.Add(vi.z);
                //        });

                //        f.Mesh.Facets.ForEach((fc) =>
                //        {
                //            int vstride = 0;
                //            if (v.Count != 0)
                //            {
                //                if ((v.Count % 3) != 0)
                //                    throw new Exception("Vertex parse error");
                //                else
                //                    vstride = v.Count / 3;
                //            }

                //            i.Add(vstride + fc.V1);
                //            i.Add(vstride + fc.V2);
                //            i.Add(vstride + fc.V3);

                //        });

                //    });



                    


                //    foreach (var inst in def.Instances)
                //    {

                //    }
                //}

                modelingManager.CreateCustomData(openFileDialog1.FileName, ddata as object);


                string cc= modelingManager.GetNumberOfDrawables();

                  Data.Eventaggregator.GetEvent<Viewer.Infrastructure.StatusBarMessageEvent>().Publish("Drawables Count " + cc);


            }
            catch (Exception ex)
            {

                System.Windows.Forms.MessageBox.Show("Import Failed " + ex.ToString());

            }
        }

        public override void CreateDocumentSpecificTools()
        {
            //if (Data.DocumentTypesLoaded.ContainsKey(this.GetType().ToString()))
            //    return;


            Data.RegisterTool("Properties", this.GetType().ToString(), Data.GetIconPath("propertiesBrowser.png"), typeof(PropertiesViewModel), RDocumentData.ToolsDockPosition.Right);

            Data.RegisterTool("TreeView", this.GetType().ToString(), Data.GetIconPath("treeview.png"), typeof(TreepanelViewModel), RDocumentData.ToolsDockPosition.Left);
            Data.RegisterTool("Shader_Editor", this.GetType().ToString(), Data.GetIconPath("shadereditor.png"), typeof(ShaderEditorViewModel), RDocumentData.ToolsDockPosition.Bottom);
            Data.RegisterTool("Output", this.GetType().ToString(), Data.GetIconPath("output.png"), typeof(OutputPaneViewModel), RDocumentData.ToolsDockPosition.Bottom);

            Data.RegisterTool("LightsCreator", this.GetType().ToString(), Data.GetIconPath("lightscreator.png"), typeof(LightCreatorViewModel), RDocumentData.ToolsDockPosition.Left);


            //propertiesBrowser

        }

        public IRModelingManager GetModelingManager()
        {
            return GraphicsEditor.GetModelingManager();
        }

        public IRMaterialManager GetMaterialManager()
        {
            return GraphicsEditor.GetMaterialManager();
        }

        public void SetSelectiontype(SelectionType type)
        {
            GraphicsEditor.SetSelectionType(type);
        }

        public IRLightManager GetLightsManager()
        {
            return GraphicsEditor.GetLightsManager();
        }
    }
}
