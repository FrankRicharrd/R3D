using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Events;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Interfaces;
using ICSharpCode.AvalonEdit.Document;
using System.Windows.Input;
using Prism.Commands;
using System.IO;
using System.Collections.ObjectModel;
using System.Windows.Forms;
using Viewer.Infrastructure;

namespace Viewer.Tools.ViewModels
{

    public class ShaderFileInfo
    {

        public string FileName { get; set; }
        public string FilePath { get; set; }

        public ShaderFileInfo(string filepath)
        {

            FilePath = filepath;
            FileName = Path.GetFileName(FilePath);
        }
    }

    public class ShaderEditorViewModel : ToolViewModel
    {
        TextDocument _document;

        ObservableCollection<ShaderFileInfo> _shaderfiles = new ObservableCollection<ShaderFileInfo>();

        public IEnumerable<ShaderType> ShaderTypes
        {
            get
            {
                return Enum.GetValues(typeof(ShaderType)).Cast<ShaderType>();
            }
        }


        IRBuilderDocument _rdoc;

        ShaderType _currentShaderType;

        public ICommand NewShaderDocumentCommand { get; set; }
        public ICommand SaveShaderDocumentCommand { get; set; }
        public ICommand OpenShaderDocumentCommand { get; set; }

        public ICommand CompileShaderCommand { get; set; }

        public ICommand SetSelectedShaderToSelectedNodeCommand { get; set; }

        ShaderFileInfo _selectedShaderFile;


        bool isNewFile = false;
        string currentFilePath = "";

        public ShaderEditorViewModel() : base("ShaderEditor")
        {
            _document = new TextDocument();

            SaveShaderDocumentCommand = new DelegateCommand(SaveShaderDocumentCommandHandler);
            OpenShaderDocumentCommand = new DelegateCommand(OpenShaderDocumentCommandHandler);

            NewShaderDocumentCommand = new DelegateCommand(NewshaderDocumentCommandHandler);
            SetSelectedShaderToSelectedNodeCommand = new DelegateCommand(SetSelectedShaderToSelectedNodeCommandHandler);

            CompileShaderCommand = new DelegateCommand(CompileShaderCommandHandler);

            MinWidth = 500;
            MinHeight = 300;
        }

        private void CompileShaderCommandHandler()
        {

        }

        private void OpenShaderDocumentCommandHandler()
        {
            OpenFileDialog openFileDialog1 = new OpenFileDialog();

            openFileDialog1.InitialDirectory = ConstantNames.ShadersDirectory;
            openFileDialog1.Title = "Browse r3d shader files";

            openFileDialog1.CheckFileExists = true;
            openFileDialog1.CheckPathExists = true;

            openFileDialog1.DefaultExt = ConstantNames.ShaderExtension;
            openFileDialog1.Filter = "r3d Shader files (*." + ConstantNames.ShaderExtension + ")|*." + ConstantNames.ShadersDirectory + "|All files (*.*)|*.*";
            openFileDialog1.FilterIndex = 2;
            openFileDialog1.RestoreDirectory = true;
            openFileDialog1.ReadOnlyChecked = true;
            openFileDialog1.ShowReadOnly = true;
            openFileDialog1.Multiselect = false;

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                currentFilePath = openFileDialog1.FileName;
                Document.Text = File.ReadAllText(openFileDialog1.FileName);
            }
        }

        private void SaveShaderDocumentCommandHandler()
        {

            if (isNewFile || string.IsNullOrEmpty(currentFilePath))
            {
                SaveFileDialog saveFileDialog1 = new SaveFileDialog();
                saveFileDialog1.InitialDirectory = ConstantNames.ShadersDirectory;
                saveFileDialog1.Title = "Save R3D Shader Files";
                //saveFileDialog1.CheckFileExists = true;
                //saveFileDialog1.CheckPathExists = true;
                saveFileDialog1.DefaultExt = ConstantNames.ShaderExtension;
                //saveFileDialog1.Filter = "r3d Shader Files (*.txt)|*.txt|All files (*.*)|*.*";
                saveFileDialog1.FilterIndex = 2;
                saveFileDialog1.RestoreDirectory = true;

                if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                {
                    string content = Document.Text;

                    File.WriteAllText(saveFileDialog1.FileName, content);

                    Shaderfiles.Add(new ShaderFileInfo(saveFileDialog1.FileName));

                    (_data.ActiveDocument as IRBuilderDocument).AddShaderPathToCurrentShaderLibrary(saveFileDialog1.FileName);
                }
            }
            else
            {
                string content = Document.Text;

                File.WriteAllText(currentFilePath, content);

                Shaderfiles.Add(new ShaderFileInfo(currentFilePath));

                (_data.ActiveDocument as IRBuilderDocument).AddShaderPathToCurrentShaderLibrary(currentFilePath);
            }
        }
        public override bool Disconnect()
        {
            return true;
        }
        private void SetSelectedShaderToSelectedNodeCommandHandler()
        {
            if (_rdoc.SelectedNodes.Count == 0)
            {
                _rdoc.GetMaterialManager().SetShaderToNode(_document.Text, CurrentShaderType, null);
            }

            _rdoc.SelectedNodes.ForEach((item) =>
            {
                _rdoc.GetMaterialManager().SetShaderToNode(_document.Text, CurrentShaderType, item);

            });


        }

        private void NewshaderDocumentCommandHandler()
        {
            isNewFile = true;
            Document = new TextDocument();
            currentFilePath = "";
        }

        public TextDocument Document
        {
            get
            {
                return _document;
            }

            set
            {
                _document = value;
                RaisePropertyChanged();
            }
        }

        public ObservableCollection<ShaderFileInfo> Shaderfiles
        {
            get
            {
                return _shaderfiles;
            }

            set
            {
                _shaderfiles = value;
                RaisePropertyChanged();
            }
        }

        public ShaderFileInfo SelectedShaderFile
        {
            get
            {
                return _selectedShaderFile;
            }

            set
            {
                _selectedShaderFile = value;

                if (File.Exists(_selectedShaderFile.FilePath))
                {
                    Document.Text = File.ReadAllText(_selectedShaderFile.FilePath);
                }

                RaisePropertyChanged();
            }
        }

        public ShaderType CurrentShaderType
        {
            get
            {
                return _currentShaderType;
            }

            set
            {
                _currentShaderType = value;
                RaisePropertyChanged();
            }
        }

        IApplicationData _data;
        IEventAggregator _event;
        public override bool Connect(IApplicationData data, IEventAggregator eventaggre)
        {
            _data = data;
            _event = eventaggre;

            (data.ActiveDocument as IRBuilderDocument).GetShaderPaths().ForEach((item) =>
            {
                Shaderfiles.Add(new ShaderFileInfo(item));
            });


            _rdoc = _data.ActiveDocument as IRBuilderDocument;

            return true;
        }
    }
}
