using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using Viewer.Interfaces;

namespace Viewer.Commands
{
    public class NewDocumentButtonClick : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            object[] objs = new object[1];
            objs[0] = _data;

            var instance = (IRDocument)System.Activator.CreateInstance(_documentType,objs);

            if (instance == null)
                return;

            _data.AddandActiveDocument(instance, true);
        }


        Type _documentType;
        IApplicationData _data;
        public NewDocumentButtonClick(Type documentType, IApplicationData data)
        {
            _documentType = documentType;
            _data = data;
        }

    }
}
