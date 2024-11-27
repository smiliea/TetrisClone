object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'TetrisClone'
  ClientHeight = 436
  ClientWidth = 436
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
    Left = 328
    Top = 16
    Width = 105
    Height = 105
    Enabled = False
  end
  object LevelPaintBox: TPaintBox
    Left = 8
    Top = 16
    Width = 105
    Height = 105
    Enabled = False
  end
  object ScorePaintBox: TPaintBox
    Left = 8
    Top = 136
    Width = 105
    Height = 105
    Enabled = False
  end
  object GameBoardPaintBox: TPaintBox
    Left = 128
    Top = 16
    Width = 185
    Height = 322
    Enabled = False
  end
  object InfoPaintBox: TPaintBox
    Left = 8
    Top = 344
    Width = 425
    Height = 89
  end
  object Timer: TTimer
    Enabled = False
    Interval = 750
    OnTimer = TimerTimer
    Left = 352
    Top = 160
  end
end
