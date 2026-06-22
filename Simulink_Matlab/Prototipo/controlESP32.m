function D = controlESP32(Vout)

persistent lastD

if isempty(lastD)
    lastD = 0.5;
end

% Valor por defecto si algo falla
D = lastD;

% Asegurar que Vout es numérico válido
Vout = double(Vout);

if ~isfinite(Vout)
    return
end

try
    % Obtener objeto serial desde workspace base
    esp = evalin("base","esp");

    % Enviar Vout al ESP32
    mensaje = sprintf("%.4f", Vout);
    writeline(esp, mensaje);

    % Leer respuesta del ESP32
    respuesta = readline(esp);
    respuesta = char(respuesta);

    D_new = str2double(strtrim(respuesta));

    % Validación del duty recibido
    if ~isnan(D_new) && isfinite(D_new)
        D_new = max(0.35, min(0.55, D_new));
        D = D_new;
        lastD = D_new;
    end

catch
    % Si falla la comunicación, mantiene el último duty válido
    D = lastD;
end

end