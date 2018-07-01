using System;
using System.Collections.Generic;
using System.Linq;
using System.ComponentModel;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Windows.Input;
using ViewerCommon;

namespace ViewerApp
{
    public abstract class ViewModelBase : INotifyPropertyChanged
    {
        private ICommand _openUriCommand;

        protected virtual bool ThrowOnInvalidPropertyName
        {
            get
            {
                return false;
            }
        }

        public ICommand OpenUriCommand
        {
            get
            {
                ICommand command = this._openUriCommand;
                if (command != null)
                    return command;
                Action<string> action = (Action<string>)(uri => Process.Start(uri));
                
                return this._openUriCommand = (ICommand)new RelayCommand<string>(execute, (Predicate<string>)(uri => Uri.IsWellFormedUriString(uri, UriKind.Absolute)));
            }
        }

        public void execute(string text)
        {
            

        }

        public event PropertyChangedEventHandler PropertyChanged;

        [Conditional("DEBUG")]
        [DebuggerStepThrough]
        public void VerifyPropertyName(string propertyName)
        {
            if (string.IsNullOrEmpty(propertyName) || Enumerable.Any<PropertyInfo>((IEnumerable<PropertyInfo>)this.GetType().GetProperties(BindingFlags.Instance | BindingFlags.Public), (Func<PropertyInfo, bool>)(x =>
            {
                if (x.Name == propertyName)
                    return x.MemberType == MemberTypes.Property;
                return false;
            })))
                return;
            string message = string.Format("Invalid property name: {0}", (object)propertyName);
            if (this.ThrowOnInvalidPropertyName)
                throw new ArgumentException(message, propertyName);
        }

        protected void OnPropertyChanged(string propertyName)
        {
            // ISSUE: reference to a compiler-generated field
            PropertyChangedEventHandler changedEventHandler = this.PropertyChanged;
            if (changedEventHandler == null)
                return;
            PropertyChangedEventArgs e = new PropertyChangedEventArgs(propertyName);
            changedEventHandler((object)this, e);
        }

        protected bool SetProperty<T>(ref T property, T newValue, [CallerMemberName] string propertyName = null)
        {
            if (object.Equals((object)property, (object)newValue))
                return false;
            property = newValue;
            this.OnPropertyChanged(propertyName);
            return true;
        }
    }
}
