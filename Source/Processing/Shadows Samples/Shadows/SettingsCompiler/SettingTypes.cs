﻿using System;
using System.Reflection;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;

namespace SettingsCompiler
{
    public enum SettingType
    {
        Float,
        Int,
        Bool,
        Enum,
        Direction,
        Orientation,
        Color,
    }

    public struct EnumValue
    {
        public string Name;
        public string Label;

        public EnumValue(string name, string label)
        {
            this.Name = name;
            this.Label = label;
        }
    }

    public abstract class Setting
    {
        public string Name;
        public string DisplayName = "";
        public SettingType Type;
        public string Group = "";
        public String HelpText = "";
        public bool UseAsShaderConstant = true;

        public Setting(FieldInfo field, SettingType type, string group)
        {
            Type = type;
            Name = field.Name;
            Group = group;
            DisplayNameAttribute.GetDisplayName(field, ref DisplayName);
            HelpTextAttribute.GetHelpText(field, ref HelpText);
            GroupAttribute.GetGroup(field, ref Group);
            UseAsShaderConstant = UseAsShaderConstantAttribute.UseFieldAsShaderConstant(field);
        }

        public abstract void WriteDeclaration(List<string> lines);
        public abstract void WriteDefinition(List<string> lines);
        public abstract void WriteInitialization(List<string> lines);

        public void WriteHLSL(List<string> lines)
        {
            if(UseAsShaderConstant == false)
                return;

            string typeString = "";
            switch(Type)
            {
                case SettingType.Enum:
                case SettingType.Int:
                    typeString = "int";
                    break;
                case SettingType.Bool:
                    typeString = "bool";
                    break;
                case SettingType.Float:
                    typeString = "float";
                    break;
                case SettingType.Direction:
                case SettingType.Color:
                    typeString = "float3";
                    break;
                case SettingType.Orientation:
                    typeString = "float4";
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }

            lines.Add("    " + typeString + " " + Name + ";");
        }

        public void WriteCBufferStruct(List<string> lines, ref uint cbSize)
        {
            if(UseAsShaderConstant == false)
                return;

            string typeString = "";
            switch(Type)
            {
                case SettingType.Enum:
                case SettingType.Int:
                    typeString = "int32";
                    cbSize += 1;
                    break;
                case SettingType.Bool:
                    typeString = "bool32";
                    cbSize += 1;
                    break;
                case SettingType.Float:
                    typeString = "float";
                    cbSize += 1;
                    break;
                case SettingType.Direction:
                case SettingType.Color:
                    typeString = "Float3";
                    if(cbSize % 4 > 1)
                        typeString = "Float4Align " + typeString;
                    cbSize += 3;
                    break;
                case SettingType.Orientation:
                    typeString = "Float4Align Quaternion";
                    cbSize += 4;
                    break;
                default:
                    Debug.Assert(false);
                    break;
            }

            lines.Add("        " + typeString + " " + Name + ";");
        }

        public void WriteCBufferUpdate(List<string> lines)
        {
            if(UseAsShaderConstant == false)
                return;

            lines.Add(string.Format("        CBuffer.Data." + Name + " = {0};", Name));
        }

        public static string FloatString(float num)
        {
            return num.ToString("F4", CultureInfo.InvariantCulture) + "f";
        }

        public static string MakeParameter(float parameter)
        {
            return ", " + FloatString(parameter);
        }

        public static string MakeParameter(string parameter)
        {
            return ", \"" + parameter + "\"";
        }

        public static string MakeParameter(int parameter)
        {
            return ", " + parameter;
        }

        public static string MakeParameter(bool parameter)
        {
            return ", " + parameter.ToString().ToLower();
        }

        public static string MakeParameter(Direction parameter)
        {
            return ", Float3(" + FloatString(parameter.X) + ", " +
                                 FloatString(parameter.Y) + ", " +
                                 FloatString(parameter.Z) + ")";
        }

        public static string MakeParameter(Orientation parameter)
        {
            return ", Quaternion(" + FloatString(parameter.X) + ", " +
                                     FloatString(parameter.Y) + ", " +
                                     FloatString(parameter.Z) + ", " +
                                     FloatString(parameter.W) + ")";
        }

        public static string MakeParameter(Color parameter)
        {
            return ", Float3(" + FloatString(parameter.R) + ", " +
                                 FloatString(parameter.G) + ", " +
                                 FloatString(parameter.B) + ")";
        }
    }

    public class FloatSetting : Setting
    {
        public float Value = 0.0f;
        public float MinValue = float.MinValue;
        public float MaxValue = float.MaxValue;
        public float StepSize = 0.01f;

        public FloatSetting(float value, FieldInfo field, string group)
            : base(field, SettingType.Float, group)
        {
            Value = value;
            MinValueAttribute.GetMinValue(field, ref MinValue);
            MaxValueAttribute.GetMaxValue(field, ref MaxValue);
            StepSizeAttribute.GetStepSize(field, ref StepSize);
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern FloatSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    FloatSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            paramString += MakeParameter(MinValue);
            paramString += MakeParameter(MaxValue);
            paramString += MakeParameter(StepSize);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class IntSetting : Setting
    {
        public int Value = 0;
        public int MinValue = int.MinValue;
        public int MaxValue = int.MaxValue;

        public IntSetting(int value, FieldInfo field, string group)
            : base(field, SettingType.Int, group)
        {
            Value = value;
            MinValueAttribute.GetMinValue(field, ref MinValue);
            MaxValueAttribute.GetMaxValue(field, ref MaxValue);
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern IntSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    IntSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            paramString += MakeParameter(MinValue);
            paramString += MakeParameter(MaxValue);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class BoolSetting : Setting
    {
        public bool Value = false;

        public BoolSetting(bool value, FieldInfo field, string group)
            : base(field, SettingType.Bool, group)
        {
            Value = value;
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern BoolSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    BoolSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class EnumSetting : Setting
    {
        public object Value;
        public Type EnumType;
        public string EnumTypeName;
        public int NumEnumValues = 0;

        public EnumSetting(object value, FieldInfo field, Type enumType, string group)
            : base(field, SettingType.Enum, group)
        {
            Value = value;
            EnumType = enumType;
            NumEnumValues = EnumType.GetEnumValues().Length;
            EnumTypeName = EnumType.Name;
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern " + EnumTypeName + "Setting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    " + EnumTypeName + "Setting " + Name + ";");
        }

        private string MakeEnumParameter(object value)
        {
            string parameter = EnumTypeName + "::" + EnumType.GetEnumName(value);
            return ", " + parameter;
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeEnumParameter(Value);
            paramString += MakeParameter(NumEnumValues);
            paramString += ", " + EnumTypeName + "Labels";
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class DirectionSetting : Setting
    {
        public Direction Value = new Direction(0.0f, 0.0f, 1.0f);

        public DirectionSetting(Direction value, FieldInfo field, string group)
            : base(field, SettingType.Direction, group)
        {
            Value = value;
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern DirectionSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    DirectionSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class OrientationSetting : Setting
    {
        public Orientation Value = Orientation.Identity;

        public OrientationSetting(Orientation value, FieldInfo field, string group)
            : base(field, SettingType.Orientation, group)
        {
            Value = value;
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern OrientationSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    OrientationSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }

    public class ColorSetting : Setting
    {
        public Color Value = new Color(1.0f, 1.0f, 1.0f);
        public bool HDR = false;
        public float MinValue = float.MinValue;
        public float MaxValue = float.MaxValue;
        public float StepSize = 0.01f;

        public ColorSetting(Color value, FieldInfo field, string group)
            : base(field, SettingType.Color, group)
        {
            Value = value;
            HDR = HDRAttribute.HDRColor(field);
            MinValueAttribute.GetMinValue(field, ref MinValue);
            MaxValueAttribute.GetMaxValue(field, ref MaxValue);
            StepSizeAttribute.GetStepSize(field, ref StepSize);
        }

        public override void WriteDeclaration(List<string> lines)
        {
            lines.Add("    extern ColorSetting " + Name + ";");
        }

        public override void WriteDefinition(List<string> lines)
        {
            lines.Add("    ColorSetting " + Name + ";");
        }

        public override void WriteInitialization(List<string> lines)
        {
            string paramString = "tweakBar";
            paramString += MakeParameter(Name);
            paramString += MakeParameter(Group);
            paramString += MakeParameter(DisplayName);
            paramString += MakeParameter(HelpText);
            paramString += MakeParameter(Value);
            paramString += MakeParameter(HDR);
            paramString += MakeParameter(MinValue);
            paramString += MakeParameter(MaxValue);
            paramString += MakeParameter(StepSize);
            lines.Add("        " + Name + ".Initialize(" + paramString + ");");
            lines.Add("        Settings.AddSetting(&" + Name + ");");
            lines.Add("");
        }
    }
}