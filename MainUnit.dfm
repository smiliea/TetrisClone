object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'TetrisClone'
  ClientHeight = 536
  ClientWidth = 517
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnPaint = FormPaint
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object PaintBoxNextPiece: TPaintBox
    Left = 336
    Top = 8
    Width = 113
    Height = 145
    Enabled = False
  end
  object LevelPaintBox: TPaintBox
    Left = 8
    Top = 16
    Width = 113
    Height = 81
    Enabled = False
  end
  object ScorePaintBox: TPaintBox
    Left = 8
    Top = 120
    Width = 113
    Height = 89
    Enabled = False
  end
  object GameBoardPaintBox: TPaintBox
    Left = 140
    Top = 16
    Width = 182
    Height = 418
    Enabled = False
  end
  object InfoPaintBox: TPaintBox
    Left = 8
    Top = 440
    Width = 499
    Height = 89
  end
  object Timer: TTimer
    Enabled = False
    Interval = 750
    OnTimer = TimerTimer
    Left = 32
    Top = 344
  end
end
