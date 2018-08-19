clc; clear;
path(pathdef)
addpath(genpath(cd))

infile = fopen('.\ATT_sky2kv5.txt','r');
fseek(infile,0,'eof');
eofbyte = ftell(infile);
fseek(infile,1,'bof');

%%
cat = zeros(1e6,5);
catcnt = 0;
count = 0;
% for c1 = 1:500;
while ftell(infile) < eofbyte
    fseek(infile,-1,0);
    tmp = fread(infile,523,'char=>char')';
    mv1 = str2double(tmp(233:238));
    if mv1 > 8, continue; end
    count = count + 1

    mv2 = str2double(tmp(239:243));
    mb1 = str2double(tmp(253:258));
    mbminusv = str2double(tmp(259:264));
    mu = str2double(tmp(273:278));
    muminusb = str2double(tmp(279:284));
    m11 = str2double(tmp(449:454));
    mvminus1 = str2double(tmp(455:460));
    m21 = str2double(tmp(470:475));
    mvminus2 = str2double(tmp(476:481));
    m1minus2 = str2double(tmp(491:496));
    m31 = str2double(tmp(499:504));
    mvminus3 = str2double(tmp(505:510));
    one_dim_spectral_class = tmp(337:339);
    
%     if ~isfinite(mv1), continue; end
    mv = mv1;
    mb = NaN;
    if isfinite(mb1)
        mb = mb1;
    elseif ~isfinite(mb1) & isfinite(mbminusv)
        mb = mbminusv + mv;
    end
    m1 = NaN;
    if isfinite(m11)
        m1 = m11;
    elseif ~isfinite(m11) & isfinite(mvminus1)
        m1 = -mvminus1 + mv;
    end
    m3 = NaN;
    if isfinite(m31)
        m3 = m31;
    elseif ~isfinite(m31) & isfinite(mvminus3)
        m3 = -mvminus3 + mv;
    end
    spec1 = one_dim_spectral_class(1);
    if strcmp(spec1, 'O')
        s = 0;
    elseif strcmp(spec1, 'B')
        s = 1;
    elseif strcmp(spec1, 'A')
        s = 2;
    elseif strcmp(spec1, 'F')
        s = 3;
    elseif strcmp(spec1, 'G')
        s = 4;
    elseif strcmp(spec1, 'K')
        s = 5;
    elseif strcmp(spec1, 'M')
        s = 6;
    elseif strcmp(spec1, 'R')
        s = 7;
    elseif strcmp(spec1, 'N')
        s = 8;
    elseif strcmp(spec1, 'C')
        s = 9;
    elseif strcmp(spec1, 'S')
        s = 10;
    else
        s = NaN;
    end
    
    photometric_system_m1 = tmp(466);
    photometric_system_m2 = tmp(487);
    photometric_system_m3 = tmp(516);
    passband_m1 = tmp(467);
    passband_m2 = tmp(488);
    passband_m3 = tmp(517);
    
    blended_v_mag_flag = tmp(249);
    blended_b_mag_flag = tmp(270);
    blended_u_mag_flag = tmp(290);
    morgan_keenan_spectral_type = tmp(305:334);
    
    iau_identifier = tmp(1:27);
    sky2000_number = str2double(tmp(28:35));
    hd_number = str2double(tmp(36:43));
    sao_number = str2double(tmp(44:50));
    dm_number = str2double(tmp(51:63));
    hr_number = str2double(tmp(64:67));
    wds_number = str2double(tmp(68:83));
    ppm_number = str2double(tmp(84:90));
    star_name = tmp(99:108);
    variable_name = tmp(109:118);
    
    rah = str2double(tmp(119:120));
    ram = str2double(tmp(121:122));
    ras = str2double(tmp(123:129));
    decsign = tmp(130);
    decd = str2double(tmp(131:132));
    if strcmp(decsign,'-'), decd = -decd; end
    decm = str2double(tmp(133:134)) * sign(decd);
    decs = str2double(tmp(135:140)) * sign(decd);
    blended_position_flag = tmp(147);
    
    pmra_sec_per_year = str2double(tmp(150:157));
    pmra_arcsec_per_year = 15*pmra_sec_per_year; % = pmra_sec_per_year * (1/3600)hr * (15)deg/hr * (3600/1)arcsec/deg;
    
    pmdecsign = tmp(158);
    pmdec_arcsec_per_year = str2double(tmp(159:165));
    if strcmp(pmdecsign,'-'), pmdec_arcsec_per_year = -pmdec_arcsec_per_year; end

    radeg = 15*(rah + ram/60 + ras/3600);
    decdeg = decd + decm/60 + decs/3600;

    multistar_separation = str2double(tmp(342:348)); %arcsec
    multistar_magdif = str2double(tmp(349:353));
    nearest_neighbor_distance = str2double(tmp(374:380)); %deg
    nearest_bright_neighbor = str2double(tmp(381:387)); %deg
    skymap_id_primary_component = str2double(tmp(388:395));
    skymap_id_secondary_component = str2double(tmp(396:403));
    skymap_id_tertiary_component = str2double(tmp(404:411));
    
    var_max_mag = str2double(tmp(412:416));
    var_min_mag = str2double(tmp(417:421));
    var_amp = str2double(tmp(422:426));
    var_passband = tmp(427);
    var_period = str2double(tmp(428:435)); %days
    epoch_of_variability = str2double(tmp(436:443));
    type_of_variable_star = str2double(tmp(444:446));
    source_of_variability_data = str2double(tmp(447:448));
    
    ndx = count;
    sky2kv5{ndx,1} = [
        sky2000_number
        ];
    sky2kv5{ndx,2} = [
        iau_identifier(9:27)
        ];
    sky2kv5{ndx,3} = [
        radeg
        decdeg
        pmra_arcsec_per_year
        pmdec_arcsec_per_year
        ];
    sky2kv5{ndx,4} = [
        mv
        mb
        m1
        m3
        s
        ];
    sky2kv5{ndx,5} = {
        hd_number
        sao_number
        hr_number
        dm_number
        ppm_number
        wds_number
        star_name
        variable_name

        mv1
        mv2
        mb1
        mbminusv
        mu
        muminusb
        m11
        mvminus1
        m21
        mvminus2
        m1minus2
        m31
        mvminus3
        photometric_system_m1
        photometric_system_m2
        photometric_system_m3
        passband_m1
        passband_m2
        passband_m3

        blended_position_flag %%%%
        blended_v_mag_flag %%%%
        blended_b_mag_flag %%%%
        blended_u_mag_flag %%%%
        
        morgan_keenan_spectral_type
        one_dim_spectral_class
        
        multistar_separation %%%%
        multistar_magdif %%%%
        nearest_neighbor_distance %%%%
        nearest_bright_neighbor %%%%
        skymap_id_primary_component
        skymap_id_secondary_component
        skymap_id_tertiary_component
        
        var_max_mag %%%%%
        var_min_mag %%%%%
        var_amp %%%%
        var_passband %%%%%
        var_period %%%%%
        epoch_of_variability
        type_of_variable_star
        source_of_variability_data        
        };
    
end
fclose(infile);

